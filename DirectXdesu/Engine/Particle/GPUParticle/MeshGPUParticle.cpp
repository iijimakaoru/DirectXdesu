#include "MeshGPUParticle.h"
#include "KDirectXCommon.h"
#include "CreateBlend.h"

#include <algorithm>
#include <future>

MeshGPUParticle::MeshGPUParticle(const Timer& timer,  const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection, Emitter* emitter, MeshModel* model)
{
	model_ = model;
	Init(timer, matView, matProjection, emitter);
}

void MeshGPUParticle::Init(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection, Emitter* emitter)
{
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commndList = directXCommon->GetCommandList();
	ID3D12CommandQueue* commndQueue = directXCommon->GetCommandQueue();

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

	// リソース構築
	BuildUAV();
	BuildRootSignature();
	BuildFrameResources();
	BuildPSOs();

	// コマンドリストを1回だけリセットしてまとめて処理
	ID3D12CommandAllocator* commandAllocator = directXCommon->GetCommandAllocator().Get();
	commandAllocator->Reset();
	commndList->Reset(commandAllocator, deadListPSO_->GetPipelineState());

	// Root Signature 設定
	commndList->SetComputeRootSignature(particleRootSignature_->GetRootSignature());

	// 現在のフレームリソースを取得
	currentFrameResourceIndex = (currentFrameResourceIndex + 1) % gNumberFrameResources;
	currentFrameResource = FrameResources[currentFrameResourceIndex].get();

	UpdateMainPassCB(timer, matView, matProjection, emitter);

	// Descriptor Heap 設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { UAVHeap.Get() };
	commndList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	// RootParameter設定
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

	// Compute Dispatch
	uint32_t numThreadGroups = static_cast<uint32_t>(std::ceil(model_->GetVertices().size() / 1024.0 + 1));
	commndList->Dispatch(numThreadGroups, 1, 1);

	// 一回だけクローズ
	commndList->Close();

	// コマンドリスト実行
	ID3D12CommandList* cmdsLists[] = { commndList };
	commndQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// キューをフラッシュ
	directXCommon->FlashCommndQueue();
}

void MeshGPUParticle::Update(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection, Emitter* emitter)
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

void MeshGPUParticle::Draw(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection, Emitter* emitter)
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

void MeshGPUParticle::BuildUAV()
{
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12Device* device = directXCommon->GetDevice();

	D3D12_DESCRIPTOR_HEAP_DESC uavHeapDesc = {};
	uint32_t numDescriptors = std::min<uint32_t>(2048, (uint32_t)model_->GetVertices().size() * 2); // 必要な分だけ確保
	uavHeapDesc.NumDescriptors = numDescriptors;
	uavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	uavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(device->CreateDescriptorHeap(&uavHeapDesc, IID_PPV_ARGS(&UAVHeap)));

	std::vector<std::future<void>> futures;

	// 並列
	// Particle Pool
	{
		futures.push_back(std::async(std::launch::async, [&] { particlePool_->Create(UAVHeap.Get(), (uint32_t)model_->GetVertices().size()); }));
	}
	// Dead List
	{
		futures.push_back(std::async(std::launch::async, [&] { deadList_->Create(UAVHeap.Get(), (uint32_t)model_->GetVertices().size()); }));
	}
	// Draw List
	{
		futures.push_back(std::async(std::launch::async, [&] {drawList_->Create(UAVHeap.Get(), (uint32_t)model_->GetVertices().size()); }));
	}
	// Draw Args
	{
		futures.push_back(std::async(std::launch::async, [&] {drawArgs_->Create(UAVHeap.Get()); }));
	}
	for (auto& f : futures) f.get();  // 全ての処理を待つ

	// Mesh
	{
		MeshSRV = model_->GetVertex()->CreateDescripterSRV(UAVHeap.Get());
	}
}

void MeshGPUParticle::BuildRootSignature()
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

void MeshGPUParticle::BuildPSOs()
{
	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDevice();

	// Graphic
	{
		graphicPSO_->SetRootSignature(rootSignature_->GetRootSignature());
		graphicPSO_->CreateVertexShader(L"MeshGPUParticle/MeshGPUParticleVS.hlsl", "main");
		graphicPSO_->CreatePixelShader(L"MeshGPUParticle/MeshGPUParticlePS.hlsl", "main");
		graphicPSO_->CreateGeometryShader(L"MeshGPUParticle/MeshGPUParticleGS.hlsl", "main");
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

	std::vector<std::future<void>> psoFutures;

	// EmitCS
	{
		psoFutures.push_back(std::async(std::launch::async, [&] { 
			emitPSO_->CreateShader(L"MeshGPUParticle/MeshEmitCS.hlsl", "main");
			emitPSO_->SetRootSignature(particleRootSignature_.get());
			emitPSO_->SetFlag(D3D12_PIPELINE_STATE_FLAG_NONE);
			emitPSO_->Create(device);
			}));
	}

	// UpdateCS
	{
		psoFutures.push_back(std::async(std::launch::async, [&] {
			updatePSO_->CreateShader(L"MeshGPUParticle/MeshUpdateCS.hlsl", "main");
			updatePSO_->SetRootSignature(particleRootSignature_.get());
			updatePSO_->SetFlag(D3D12_PIPELINE_STATE_FLAG_NONE);
			updatePSO_->Create(device);
			}));
	}

	// CopyDrawCountCS
	{
		psoFutures.push_back(std::async(std::launch::async, [&] {
			copyDrawPSO_->CreateShader(L"MeshGPUParticle/MeshCopyDrawCountCS.hlsl", "main");
			copyDrawPSO_->SetRootSignature(particleRootSignature_.get());
			copyDrawPSO_->SetFlag(D3D12_PIPELINE_STATE_FLAG_NONE);
			copyDrawPSO_->Create(device);
			}));
	}

	// DeadListInitCS
	{
		psoFutures.push_back(std::async(std::launch::async, [&] {
			deadListPSO_->CreateShader(L"MeshGPUParticle/MeshDeadListInitCS.hlsl", "main");
			deadListPSO_->SetRootSignature(particleRootSignature_.get());
			deadListPSO_->SetFlag(D3D12_PIPELINE_STATE_FLAG_NONE);
			deadListPSO_->Create(device);
			}));
	}

	for (auto& f : psoFutures) f.get();
}

void MeshGPUParticle::BuildFrameResources()
{
	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDevice();
	std::vector<std::future<std::unique_ptr<FrameResource>>> frameFutures;
	for (int i = 0; i < gNumberFrameResources; ++i)
	{
		frameFutures.push_back(std::async(std::launch::async, [device]() 
			{
			return std::make_unique<FrameResource>(device, 1, 1, 1);
			}));
	}
	for (auto& f : frameFutures) FrameResources.push_back(f.get());
}

void MeshGPUParticle::UpdateMainPassCB(const Timer& timer,
	const KMyMath::Matrix4& matView,
	const KMyMath::Matrix4& matProjection,
	Emitter* emitter)
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

	MainTimeCB.DeltaTime = timer.GetDeltaTime();
	MainTimeCB.TotalTime = timer.GetTotalTime();

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

void MeshGPUParticle::ParticleUpdate()
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

void MeshGPUParticle::ParticleDraw()
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

void MeshGPUParticle::DrawCommon()
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
