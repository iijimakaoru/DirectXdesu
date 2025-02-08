#include "GPUParticle.h"
#include "KDirectXCommon.h"
#include "CreateBlend.h"

GPUParticle::GPUParticle(const Timer& timer,
	const KMyMath::Matrix4& matView,
	const KMyMath::Matrix4& matProjection,
	Emitter* emitter)
{
	Init(timer, matView, matProjection,emitter);
}

void GPUParticle::Init(const Timer& timer,
	const KMyMath::Matrix4& matView,
	const KMyMath::Matrix4& matProjection,
	Emitter* emitter)
{
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = directXCommon->GetMainCommandList();
	ID3D12CommandQueue* commandQueue = directXCommon->GetMainCommandQueue();
	ID3D12CommandAllocator* commandAllocator = directXCommon->GetMainCommandAllocator();
	ID3D12Fence* fence = KDirectXCommon::GetInstance()->GetFenceMain();

	rootSignature_ = std::make_unique<RootSignature>();
	particleRootSignature_ = std::make_unique<RootSignature>();
	graphicPSO_ = std::make_unique<GraphicPipelineState>();
	emitPSO_ = std::make_unique<ComputePipelineState>();
	updatePSO_ = std::make_unique<ComputePipelineState>();
	copyDrawPSO_ = std::make_unique<ComputePipelineState>();
	deadListPSO_ = std::make_unique<ComputePipelineState>();
	particlePool_ = std::make_unique<ParticlePool>();
	deadList_ = std::make_unique<DeadList>();
	drawList_ = std::make_unique<DrawList>();
	drawArgs_ = std::make_unique<DrawArgs>();
	commandSignature_ = std::make_unique<CommandSignature>();

	BuildUAV(emitter);
	BuildRootSignature();
	BuildFrameResources();
	BuildPSOs();

	// 初期化コマンドを実行する
	ThrowIfFailed(commandList->Close());
	ID3D12CommandList* cmdsLists[] = { commandList };
	commandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	directXCommon->FlashCommandQueue();

	ThrowIfFailed(commandAllocator->Reset());

	ThrowIfFailed(commandList->Reset(commandAllocator, deadListPSO_->GetPipelineState()));

	commandList->SetComputeRootSignature(particleRootSignature_->GetRootSignature());

	currentFrameResourceIndex = (currentFrameResourceIndex + 1) % gNumberFrameResources;
	currentFrameResource = FrameResources[currentFrameResourceIndex].get();

	UpdateMainPassCB(timer, matView, matProjection,emitter);

	ID3D12DescriptorHeap* descriptorHeaps[] = { UAVHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	auto objectCB = currentFrameResource->ObjectCB->Resource();
	commandList->SetComputeRootConstantBufferView(0, objectCB->GetGPUVirtualAddress());

	auto timeCB = currentFrameResource->TimeCB->Resource();
	commandList->SetComputeRootConstantBufferView(1, timeCB->GetGPUVirtualAddress());

	auto particleCB = currentFrameResource->ParticleCB->Resource();
	commandList->SetComputeRootConstantBufferView(2, particleCB->GetGPUVirtualAddress());

	commandList->SetComputeRootDescriptorTable(3, particlePool_->GetGPUUAV());
	commandList->SetComputeRootDescriptorTable(4, deadList_->GetGPUUAV());
	commandList->SetComputeRootDescriptorTable(5, drawList_->GetGPUUAV());
	commandList->SetComputeRootDescriptorTable(6, drawArgs_->GetGPUUAV());

	commandList->Dispatch(emitter->GetMaxParticles(), 1, 1);

	ThrowIfFailed(commandList->Close());

	// コマンドリストを実行キューに追加します。
	ID3D12CommandList* cmdsLists1[] = { commandList };
	commandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists1);

	directXCommon->FlashCommandQueue();

	directXCommon->MainCommandListReset();
}

void GPUParticle::Update(const Timer& timer,
	const KMyMath::Matrix4& matView, 
	const KMyMath::Matrix4& matProjection,
	Emitter* emitter)
{
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12Fence* fence = directXCommon->GetFenceMain();

	// 円形のフレーム リソース配列を循環します。
	currentFrameResourceIndex = (currentFrameResourceIndex + 1) % gNumberFrameResources;
	currentFrameResource = FrameResources[currentFrameResourceIndex].get();

	// GPU は現在のフレーム リソースのコマンドの処理を終了しましたか?
	// そうでない場合は、GPU がこのフェンス ポイントまでのコマンドを完了するまで待ちます
	if (currentFrameResource->Fence != 0 && fence->GetCompletedValue() < currentFrameResource->Fence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, (LPCWSTR)false, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(fence->SetEventOnCompletion(currentFrameResource->Fence, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	emitter->Update(timer.GetTotalTime());

	UpdateMainPassCB(timer, matView, matProjection,emitter);
}

void GPUParticle::Draw(const Timer& timer,
	const KMyMath::Matrix4& matView,
	const KMyMath::Matrix4& matProjection,
	Emitter* emitter)
{
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commndList = directXCommon->GetMainCommandList();

	auto currentCommandListAllocator = currentFrameResource->commandListAllocator;

	commndList->SetPipelineState(emitPSO_->GetPipelineState());
	commndList->SetComputeRootSignature(particleRootSignature_->GetRootSignature());

	ID3D12DescriptorHeap* descriptorHeaps[] = { UAVHeap.Get() };
	commndList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	auto objectCB = currentFrameResource->ObjectCB->Resource();
	commndList->SetComputeRootConstantBufferView(0, objectCB->GetGPUVirtualAddress());

	auto timeCB = currentFrameResource->TimeCB->Resource();
	commndList->SetComputeRootConstantBufferView(1, timeCB->GetGPUVirtualAddress());

	auto particleCB = currentFrameResource->ParticleCB->Resource();
	commndList->SetComputeRootConstantBufferView(2, particleCB->GetGPUVirtualAddress());

	commndList->SetComputeRootDescriptorTable(3, particlePool_->GetGPUUAV());
	commndList->SetComputeRootDescriptorTable(4, deadList_->GetGPUUAV());
	commndList->SetComputeRootDescriptorTable(5, drawList_->GetGPUUAV());
	commndList->SetComputeRootDescriptorTable(6, drawArgs_->GetGPUUAV());

	while (emitter->GetEmitTimeCounter() >= emitter->GetTimeBetweenEmit())
	{
		emitter->SetEmitCount((int)(emitter->GetEmitTimeCounter() / emitter->GetTimeBetweenEmit()));

		emitter->SetEmitCount(min(emitter->GetEmitCount(), 65535));
		emitter->SetEmitTimeCounter(fmod(emitter->GetEmitTimeCounter(), emitter->GetTimeBetweenEmit()));

		UpdateMainPassCB(timer, matView, matProjection,emitter);

		commndList->Dispatch(emitter->GetEmitCount(), 1, 1);
	}

	CD3DX12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(drawList_->GetDrawList(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_DEST);
	commndList->ResourceBarrier(1, &resourceBarrier);

	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(drawList_->GetDrawList(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	commndList->ResourceBarrier(1, &resourceBarrier);

	// パーティクル更新シェーダー
	commndList->SetPipelineState(updatePSO_->GetPipelineState());
	commndList->SetComputeRootSignature(particleRootSignature_->GetRootSignature());
	commndList->Dispatch(emitter->GetMaxParticles(), 1, 1);

	resourceBarrier = CD3DX12_RESOURCE_BARRIER::UAV(drawList_->GetDrawList());
	commndList->ResourceBarrier(1, &resourceBarrier);

	// パーティクル描画シェーダー
	commndList->SetPipelineState(copyDrawPSO_->GetPipelineState());
	commndList->SetComputeRootSignature(particleRootSignature_->GetRootSignature());
	commndList->Dispatch(1, 1, 1);

	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(drawList_->GetDrawList(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	commndList->ResourceBarrier(1, &resourceBarrier);

	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(particlePool_->GetParticlePool(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	commndList->ResourceBarrier(1, &resourceBarrier);

	commndList->SetPipelineState(graphicPSO_->GetPipelineState());

	commndList->SetGraphicsRootSignature(rootSignature_->GetRootSignature());

	commndList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	commndList->SetGraphicsRootConstantBufferView(0, objectCB->GetGPUVirtualAddress());
	commndList->SetGraphicsRootConstantBufferView(1, timeCB->GetGPUVirtualAddress());
	commndList->SetGraphicsRootConstantBufferView(2, particleCB->GetGPUVirtualAddress());

	commndList->SetGraphicsRootDescriptorTable(3, particlePool_->GetGPUSRV());
	commndList->SetGraphicsRootDescriptorTable(4, drawList_->GetGPUSRV());

	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(drawArgs_->GetDrawArgs(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT);
	commndList->ResourceBarrier(1, &resourceBarrier);

	commndList->ExecuteIndirect(commandSignature_->GetCommandSignature(), 1, drawArgs_->GetDrawArgs(), 0, nullptr, 0);

	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(drawArgs_->GetDrawArgs(),
		D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	commndList->ResourceBarrier(1, &resourceBarrier);

	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(drawList_->GetDrawList(),
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	commndList->ResourceBarrier(1, &resourceBarrier);

	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(particlePool_->GetParticlePool(),
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	commndList->ResourceBarrier(1, &resourceBarrier);
}

void GPUParticle::BuildUAV(Emitter* emitter)
{
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12Device* device = directXCommon->GetDevice();

	D3D12_DESCRIPTOR_HEAP_DESC uavHeapDesc = {};
	uavHeapDesc.NumDescriptors = 2048;
	uavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	uavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(device->CreateDescriptorHeap(&uavHeapDesc, IID_PPV_ARGS(&UAVHeap)));

	// Particle Pool
	{
		particlePool_->Create(UAVHeap.Get(), (uint32_t)emitter->GetMaxParticles());
	}

	// Dead List
	{
		deadList_->Create(UAVHeap.Get(), (uint32_t)emitter->GetMaxParticles());
	}

	// Draw List
	{
		drawList_->Create(UAVHeap.Get(), (uint32_t)emitter->GetMaxParticles());
	}

	// Draw Args
	{
		drawArgs_->Create(UAVHeap.Get());
	}
}

void GPUParticle::BuildRootSignature()
{
	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDevice();

	// デフォルトのルート署名
	{
		rootSignature_->Add(RootType::CBV, 0);// b0
		rootSignature_->Add(RootType::CBV, 1);// b1
		rootSignature_->Add(RootType::CBV, 2);// b2
		rootSignature_->Add(RangeType::SRV, 0);
		rootSignature_->Add(RangeType::SRV, 1);

		rootSignature_->AddStaticSampler(0, 0, D3D12_FILTER_MIN_MAG_MIP_POINT);
		rootSignature_->AddStaticSampler(1, 0, D3D12_FILTER_MIN_MAG_MIP_POINT, AddressMode::Clamp, AddressMode::Clamp, AddressMode::Clamp);
		rootSignature_->AddStaticSampler(2);
		rootSignature_->AddStaticSampler(3, 0, D3D12_FILTER_MIN_MAG_MIP_LINEAR, AddressMode::Clamp, AddressMode::Clamp, AddressMode::Clamp);
		rootSignature_->AddStaticSampler(4, 0, D3D12_FILTER_ANISOTROPIC);
		rootSignature_->AddStaticSampler(5, 0, D3D12_FILTER_ANISOTROPIC, AddressMode::Clamp, AddressMode::Clamp, AddressMode::Clamp);

		rootSignature_->Create(device);
	}

	// 粒子ルート署名
	{
		particleRootSignature_->Add(RootType::CBV, 0);
		particleRootSignature_->Add(RootType::CBV, 1);
		particleRootSignature_->Add(RootType::CBV, 2);
		particleRootSignature_->Add(RangeType::UAV, 0);
		particleRootSignature_->Add(RangeType::UAV, 1);
		particleRootSignature_->Add(RangeType::UAV, 2);
		particleRootSignature_->Add(RangeType::UAV, 3);
		particleRootSignature_->Add(RangeType::SRV, 0);

		particleRootSignature_->AddStaticSampler(0);

		particleRootSignature_->Create(device);
	}

	// パーティクルコマンドシグネチャ
	commandSignature_->Create();
}

void GPUParticle::BuildPSOs()
{
	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDevice();

	// Graphic
	{
		graphicPSO_->SetRootSignature(rootSignature_->GetRootSignature());
		graphicPSO_->CreateVertexShader(L"GPUParticle/GPUParticleVS.hlsl", "main");
		graphicPSO_->CreatePixelShader(L"GPUParticle/GPUParticlePS.hlsl", "main");
		graphicPSO_->CreateGeometryShader(L"GPUParticle/GPUParticleGS.hlsl", "main");
	}

	// Blend
	D3D12_BLEND_DESC lBlendDesc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	lBlendDesc = CreateBlend(BlendMode::ADD);

	// RenderTargetFormat
	RenderTargetFormat renderTargetFormat{};
	renderTargetFormat.NumRenderTargets = 1;
	renderTargetFormat.RTVFormats[0] = BackBufferFormat;

	// Graphic
	{
		graphicPSO_->SetPrimitiveType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
		graphicPSO_->SetRenderTargetFormat(renderTargetFormat);
		graphicPSO_->SetDepthFlag(false);
		graphicPSO_->SetDepthWriteMask(D3D12_DEPTH_WRITE_MASK_ZERO);
		graphicPSO_->SetFillMode(D3D12_FILL_MODE_SOLID);
		graphicPSO_->SetBlend(lBlendDesc);
		graphicPSO_->Create(device);
	}

	// EmitCS
	{
		emitPSO_->CreateShader(L"GPUParticle/EmitCS.hlsl", "main");
		emitPSO_->SetRootSignature(particleRootSignature_.get());
		emitPSO_->SetFlag(D3D12_PIPELINE_STATE_FLAG_NONE);
		emitPSO_->Create(device);
	}

	// UpdateCS
	{
		updatePSO_->CreateShader(L"GPUParticle/UpdateCS.hlsl", "main");
		updatePSO_->SetRootSignature(particleRootSignature_.get());
		updatePSO_->SetFlag(D3D12_PIPELINE_STATE_FLAG_NONE);
		updatePSO_->Create(device);
	}

	// CopyDrawCountCS
	{
		copyDrawPSO_->CreateShader(L"GPUParticle/CopyDrawCountCS.hlsl", "main");
		copyDrawPSO_->SetRootSignature(particleRootSignature_.get());
		copyDrawPSO_->SetFlag(D3D12_PIPELINE_STATE_FLAG_NONE);
		copyDrawPSO_->Create(device);
	}

	// DeadListInitCS
	{
		deadListPSO_->CreateShader(L"GPUParticle/DeadListInitCS.hlsl", "main");
		deadListPSO_->SetRootSignature(particleRootSignature_.get());
		deadListPSO_->SetFlag(D3D12_PIPELINE_STATE_FLAG_NONE);
		deadListPSO_->Create(device);
	}
}

void GPUParticle::BuildFrameResources()
{
	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDevice();

	for (int i = 0; i < gNumberFrameResources; ++i)
	{
		FrameResources.push_back(std::make_unique<FrameResource>(device, 1, 1, 1));
	}
}

void GPUParticle::UpdateMainPassCB(const Timer& timer,
	const KMyMath::Matrix4& matView,
	const KMyMath::Matrix4& matProjection,
	Emitter* emitter)
{
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX view = MyMathConvert::ChangeMatrix4toXMMATRIX(matView);
	DirectX::XMMATRIX projection = MyMathConvert::ChangeMatrix4toXMMATRIX(matProjection);

	ObjectConstants objConstants;
	XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
	XMStoreFloat4x4(&objConstants.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&objConstants.Projection, XMMatrixTranspose(projection));
	objConstants.AspectRatio = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();

	auto currentObjectCB = currentFrameResource->ObjectCB.get();
	currentObjectCB->CopyData(0, objConstants);

	MainTimeCB.DeltaTime = timer.GetDeltaTime();
	MainTimeCB.TotalTime = timer.GetTotalTime();

	auto currentTimeCB = currentFrameResource->TimeCB.get();
	currentTimeCB->CopyData(0, MainTimeCB);

	MainParticleCB.EmitCount = emitter->GetEmitCount();
	MainParticleCB.MaxParticles = emitter->GetMaxParticles();
	MainParticleCB.GridSize = emitter->GetGridSize();
	MainParticleCB.LifeTime = emitter->GetLifeTime();
	MainParticleCB.velocity = emitter->GetVelocity();
	MainParticleCB.acceleration = emitter->GetAcceleration();

	auto currentParticleCB = currentFrameResource->ParticleCB.get();
	currentParticleCB->CopyData(0, MainParticleCB);
}

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GPUParticle::GetStaticSamplers()
{
	// Applications usually only need a handful of samplers.  So just define them all up front
	// and keep them available as part of the root signature.  

	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy

	return {
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp };
}
