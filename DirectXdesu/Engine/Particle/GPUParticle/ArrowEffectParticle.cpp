#include "ArrowEffectParticle.h"
#include "KDirectXCommon.h"
#include "CreateBlend.h"

#include <algorithm>
#include <future>

ArrowEffectParticle::ArrowEffectParticle(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection, Emitter* emitter, MeshModel* model)
{
	model_ = model;
	Init(timer, matView, matProjection, emitter);
}

void ArrowEffectParticle::Init(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection, Emitter* emitter)
{
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commndList = directXCommon->GetCommandList();
	ID3D12CommandQueue* commndQueue = directXCommon->GetCommandQueue();

	directXCommon->BeginCommnd();

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

	BuildUAV();
	BuildRootSignature();
	BuildFrameResources();
	BuildPSOs();

	directXCommon->GetCommandList()->SetPipelineState(deadListPSO_->GetPipelineState());
	directXCommon->GetCommandList()->SetComputeRootSignature(particleRootSignature_->GetRootSignature());

	currentFrameResourceIndex = (currentFrameResourceIndex + 1) % gNumberFrameResources;
	currentFrameResource = FrameResources[currentFrameResourceIndex].get();

	UpdateMainPassCB(timer, matView, matProjection, emitter);

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
	commndList->SetComputeRootDescriptorTable(7, MeshSRV);

	commndList->Dispatch(static_cast<uint32_t>(model_->GetVertices().size() / 1024 + 1), 1, 1);

	directXCommon->CloseCommnd();
}

void ArrowEffectParticle::Update(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection, Emitter* emitter)
{
	ID3D12Fence* fence = KDirectXCommon::GetInstance()->GetFence();

	// 円形のフレーム リソース配列を循環します
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

	UpdateMainPassCB(timer, matView, matProjection, emitter);
}

void ArrowEffectParticle::Draw(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection, Emitter* emitter)
{
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commndList = directXCommon->GetCommandList();

	auto currentCommandListAllocator = currentFrameResource->commandListAllocator;

	if (!init)
	{
		init = true;
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
		commndList->SetComputeRootDescriptorTable(7, MeshSRV);

		UpdateMainPassCB(timer, matView, matProjection, emitter);
		commndList->Dispatch(static_cast<uint32_t>(model_->GetVertices().size() / 1024 + 1), 1, 1);
	}

	drawList_->Translation(commndList, D3D12_RESOURCE_STATE_COPY_DEST);

	drawList_->Translation(commndList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	// パーティクル更新シェーダー
	ParticleUpdate();

	CD3DX12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::UAV(drawList_->GetDrawList());
	commndList->ResourceBarrier(1, &resourceBarrier);

	// パーティクル描画シェーダー
	ParticleDraw();

	drawList_->Translation(commndList, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

	particlePool_->Translation(commndList, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

	// 描画
	DrawCommon();

	drawArgs_->Translation(commndList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	drawList_->Translation(commndList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	particlePool_->Translation(commndList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
}

void ArrowEffectParticle::BuildUAV()
{
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12Device* device = directXCommon->GetDevice();

	D3D12_DESCRIPTOR_HEAP_DESC uavHeapDesc = {};
	uint32_t numDescriptors = std::min<uint32_t>(2048, (uint32_t)model_->GetVertices().size() * 2); // 必要な分だけ確保
	uavHeapDesc.NumDescriptors = numDescriptors;
	uavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	uavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(device->CreateDescriptorHeap(&uavHeapDesc, IID_PPV_ARGS(&UAVHeap)));


	// 並列
	// Particle Pool
	{
		particlePool_->Create(UAVHeap.Get(), (uint32_t)model_->GetVertices().size());
	}
	// Dead List
	{
		deadList_->Create(UAVHeap.Get(), (uint32_t)model_->GetVertices().size());
	}
	// Draw List
	{
		drawList_->Create(UAVHeap.Get(), (uint32_t)model_->GetVertices().size());
	}
	// Draw Args
	{
		drawArgs_->Create(UAVHeap.Get());
	}

	// Mesh
	{
		MeshSRV = model_->GetVertex()->CreateDescripterSRV(UAVHeap.Get());
	}
}

void ArrowEffectParticle::BuildRootSignature()
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

void ArrowEffectParticle::BuildPSOs()
{
	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDevice();

	// Graphic
	{
		graphicPSO_->SetRootSignature(rootSignature_->GetRootSignature());
		graphicPSO_->CreateVertexShader(L"Effect/ArrowEffect/ArrowEffectVS.hlsl", "main");
		graphicPSO_->CreatePixelShader(L"Effect/ArrowEffect/ArrowEffectPS.hlsl", "main");
		graphicPSO_->CreateGeometryShader(L"Effect/ArrowEffect/ArrowEffectGS.hlsl", "main");
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
		emitPSO_->CreateShader(L"Effect/ArrowEffect/ArrowEffectEmitCS.hlsl", "main");
		emitPSO_->SetRootSignature(particleRootSignature_.get());
		emitPSO_->SetFlag(D3D12_PIPELINE_STATE_FLAG_NONE);
		emitPSO_->Create(device);
	}

	// UpdateCS
	{
		updatePSO_->CreateShader(L"Effect/ArrowEffect/ArrowEffectUpdateCS.hlsl", "main");
		updatePSO_->SetRootSignature(particleRootSignature_.get());
		updatePSO_->SetFlag(D3D12_PIPELINE_STATE_FLAG_NONE);
		updatePSO_->Create(device);
	}

	// CopyDrawCountCS
	{
		copyDrawPSO_->CreateShader(L"Effect/ArrowEffect/ArrowEffectCopyDrawCountCS.hlsl", "main");
		copyDrawPSO_->SetRootSignature(particleRootSignature_.get());
		copyDrawPSO_->SetFlag(D3D12_PIPELINE_STATE_FLAG_NONE);
		copyDrawPSO_->Create(device);
	}

	// DeadListInitCS
	{
		deadListPSO_->CreateShader(L"Effect/ArrowEffect/ArrowEffectDeadListInitCS.hlsl", "main");
		deadListPSO_->SetRootSignature(particleRootSignature_.get());
		deadListPSO_->SetFlag(D3D12_PIPELINE_STATE_FLAG_NONE);
		deadListPSO_->Create(device);
	}
}

void ArrowEffectParticle::BuildFrameResources()
{
	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDevice();
	for (int i = 0; i < gNumberFrameResources; ++i)
	{
		FrameResources.push_back(std::make_unique<FrameResource>(device, 1, 1, 1));
	}
}

void ArrowEffectParticle::UpdateMainPassCB(const Timer* timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection, Emitter* emitter)
{
	DirectX::XMMATRIX matScale = DirectX::XMMatrixIdentity();
	matScale =
		DirectX::XMMatrixScaling(
			emitter->GetScaling().x,
			emitter->GetScaling().y,
			emitter->GetScaling().z);

	DirectX::XMMATRIX matRotation = DirectX::XMMatrixIdentity();
	matRotation *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(emitter->GetRotation().z));
	matRotation *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(emitter->GetRotation().x));
	matRotation *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(emitter->GetRotation().y));

	DirectX::XMMATRIX matTrans = DirectX::XMMatrixIdentity();
	matTrans =
		DirectX::XMMatrixTranslation(
			emitter->GetPosition().x,
			emitter->GetPosition().y,
			emitter->GetPosition().z);

	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	world *= matScale * matRotation * matTrans;

	DirectX::XMMATRIX view = MyMathConvert::ChangeMatrix4toXMMATRIX(matView);
	DirectX::XMMATRIX projection = MyMathConvert::ChangeMatrix4toXMMATRIX(matProjection);

	ObjectConstants objConstants;
	XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
	XMStoreFloat4x4(&objConstants.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&objConstants.Projection, XMMatrixTranspose(projection));
	objConstants.AspectRatio = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();

	auto currentObjectCB = currentFrameResource->ObjectCB.get();
	currentObjectCB->CopyData(0, objConstants);

	MainTimeCB.DeltaTime = timer->GetDeltaTime();
	MainTimeCB.TotalTime = timer->GetTotalTime();

	auto currentTimeCB = currentFrameResource->TimeCB.get();
	currentTimeCB->CopyData(0, MainTimeCB);

	MainParticleCB.EmitCount = emitter->GetEmitCount();
	MainParticleCB.MaxParticles = (uint32_t)model_->GetVertices().size();
	MainParticleCB.GridSize = emitter->GetGridSize();
	MainParticleCB.LifeTime = emitter->GetLifeTime();
	MainParticleCB.velocity = emitter->GetVelocity();
	MainParticleCB.acceleration = emitter->GetAcceleration();
	MainParticleCB.vertexNum = (uint32_t)model_->GetVertices().size();
	MainParticleCB.startColor = emitter->GetStartColor();
	MainParticleCB.endColor = emitter->GetEndColor();
	MainParticleCB.size = emitter->GetParticleSize();

	auto currentParticleCB = currentFrameResource->ParticleCB.get();
	currentParticleCB->CopyData(0, MainParticleCB);
}

void ArrowEffectParticle::ParticleUpdate()
{
	ID3D12GraphicsCommandList* commndList = KDirectXCommon::GetInstance()->GetCommandList();

	commndList->SetPipelineState(updatePSO_->GetPipelineState());
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
	commndList->SetComputeRootDescriptorTable(7, MeshSRV);

	commndList->Dispatch(static_cast<uint32_t>(model_->GetVertices().size() / 1024 + 1), 1, 1);
}

void ArrowEffectParticle::ParticleDraw()
{
	ID3D12GraphicsCommandList* commndList = KDirectXCommon::GetInstance()->GetCommandList();

	commndList->SetPipelineState(copyDrawPSO_->GetPipelineState());
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
	commndList->SetComputeRootDescriptorTable(7, MeshSRV);

	commndList->Dispatch(1, 1, 1);
}

void ArrowEffectParticle::DrawCommon()
{
	ID3D12GraphicsCommandList* commndList = KDirectXCommon::GetInstance()->GetCommandList();

	commndList->SetPipelineState(graphicPSO_->GetPipelineState());
	commndList->SetGraphicsRootSignature(rootSignature_->GetRootSignature());

	ID3D12DescriptorHeap* descriptorHeaps[] = { UAVHeap.Get() };
	commndList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	auto objectCB = currentFrameResource->ObjectCB->Resource();
	auto timeCB = currentFrameResource->TimeCB->Resource();
	auto particleCB = currentFrameResource->ParticleCB->Resource();

	commndList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	commndList->SetGraphicsRootConstantBufferView(0, objectCB->GetGPUVirtualAddress());
	commndList->SetGraphicsRootConstantBufferView(1, timeCB->GetGPUVirtualAddress());
	commndList->SetGraphicsRootConstantBufferView(2, particleCB->GetGPUVirtualAddress());
	commndList->SetGraphicsRootDescriptorTable(3, particlePool_->GetGPUSRV());
	commndList->SetGraphicsRootDescriptorTable(4, drawList_->GetGPUSRV());

	drawArgs_->Translation(commndList, D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT);

	commndList->ExecuteIndirect(commandSignature_->GetCommandSignature(), 1, drawArgs_->GetDrawArgs(), 0, nullptr, 0);
}
