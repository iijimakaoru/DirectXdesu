#include "MeshGPUParticle.h"
#include "KDirectXCommon.h"
#include "CreateBlend.h"

MeshGPUParticle::MeshGPUParticle(const Timer& timer,
	const KMyMath::Matrix4& matView,
	const KMyMath::Matrix4& matProjection,
	Emitter* emitter, const std::string modelname)
{
	Init(timer, matView, matProjection, emitter,modelname);
}

void MeshGPUParticle::Init(const Timer& timer,
	const KMyMath::Matrix4& matView,
	const KMyMath::Matrix4& matProjection,
	Emitter* emitter,
	const std::string modelname)
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

	LoadMesh(modelname);
	BuildUAV(emitter);
	BuildRootSignature();
	BuildShadersAndInputLayout();
	BuildFrameResources();
	BuildPSOs();

	// execute the initialization commands
	ThrowIfFailed(commndList->Close());
	ID3D12CommandList* cmdsLists[] = { commndList };
	commndQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	directXCommon->FlashCommndQueue();

	ThrowIfFailed(directXCommon->GetCommandAllocator()->Reset());

	ThrowIfFailed(
		directXCommon->GetCommandList()->Reset(directXCommon->GetCommandAllocator().Get(),
			deadListPSO_->GetPipelineState()));

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

	commndList->SetComputeRootDescriptorTable(3, ParticlePoolGPUUAV);
	commndList->SetComputeRootDescriptorTable(4, ACDeadListGPUUAV);
	commndList->SetComputeRootDescriptorTable(5, DrawListGPUUAV);
	commndList->SetComputeRootDescriptorTable(6, DrawArgsGPUUAV);

	commndList->Dispatch(emitter->GetMaxParticles(), 1, 1);

	ThrowIfFailed(commndList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists1[] = { commndList };
	commndQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists1);

	directXCommon->FlashCommndQueue();

	directXCommon->BeginCommnd();
}

void MeshGPUParticle::Update(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection, Emitter* emitter)
{
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12Fence* fence = directXCommon->GetFence();

	// Cycle through the circular frame resource array.
	currentFrameResourceIndex = (currentFrameResourceIndex + 1) % gNumberFrameResources;
	currentFrameResource = FrameResources[currentFrameResourceIndex].get();

	// Has the GPU finished processing the commands of the current frame resource?
	// If not, wait until the GPU has completed commands up to this fence point.
	if (currentFrameResource->Fence != 0 && fence->GetCompletedValue() < currentFrameResource->Fence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, (LPCWSTR)false, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(fence->SetEventOnCompletion(currentFrameResource->Fence, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	emitter->Update(timer.GetTotalTime());

	UpdateMainPassCB(timer, matView, matProjection, emitter);
}

void MeshGPUParticle::Draw(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection, Emitter* emitter)
{
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commndList = directXCommon->GetCommandList();

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

	commndList->SetComputeRootDescriptorTable(3, ParticlePoolGPUUAV);
	commndList->SetComputeRootDescriptorTable(4, ACDeadListGPUUAV);
	commndList->SetComputeRootDescriptorTable(5, DrawListGPUUAV);
	commndList->SetComputeRootDescriptorTable(6, DrawArgsGPUUAV);

	commndList->SetComputeRootDescriptorTable(7, MeshSRV);

	if (!init) 
	{
		init = true;
		while (emitter->GetEmitTimeCounter() >= emitter->GetTimeBetweenEmit())
		{
			emitter->SetEmitCount((int)(emitter->GetEmitTimeCounter() / emitter->GetTimeBetweenEmit()));

			emitter->SetEmitCount(emitter->GetEmitCount());
			emitter->SetEmitTimeCounter(fmod(emitter->GetEmitTimeCounter(), emitter->GetTimeBetweenEmit()));

			UpdateMainPassCB(timer, matView, matProjection, emitter);

			commndList->Dispatch(emitter->GetEmitCount(), 1, 1);
		}
	}

	CD3DX12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(RWDrawList.Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_DEST);
	commndList->ResourceBarrier(1, &resourceBarrier);

	commndList->CopyResource(RWDrawList.Get(), DrawListUploadBuffer.Get());

	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(RWDrawList.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	commndList->ResourceBarrier(1, &resourceBarrier);

	// パーティクル更新シェーダー
	commndList->SetPipelineState(updatePSO_->GetPipelineState());
	commndList->SetComputeRootSignature(particleRootSignature_->GetRootSignature());
	commndList->Dispatch(emitter->GetMaxParticles(), 1, 1);

	resourceBarrier = CD3DX12_RESOURCE_BARRIER::UAV(RWDrawList.Get());
	commndList->ResourceBarrier(1, &resourceBarrier);

	// パーティクル描画シェーダー
	commndList->SetPipelineState(copyDrawPSO_->GetPipelineState());
	commndList->SetComputeRootSignature(particleRootSignature_->GetRootSignature());
	commndList->Dispatch(1, 1, 1);

	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(RWDrawList.Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	commndList->ResourceBarrier(1, &resourceBarrier);

	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(RWParticlePool.Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	commndList->ResourceBarrier(1, &resourceBarrier);

	commndList->SetPipelineState(graphicPSO_->GetPipelineState());

	commndList->SetGraphicsRootSignature(rootSignature_->GetRootSignature());

	commndList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	commndList->SetGraphicsRootConstantBufferView(0, objectCB->GetGPUVirtualAddress());
	commndList->SetGraphicsRootConstantBufferView(1, timeCB->GetGPUVirtualAddress());
	commndList->SetGraphicsRootConstantBufferView(2, particleCB->GetGPUVirtualAddress());

	commndList->SetGraphicsRootDescriptorTable(3, ParticlePoolGPUSRV);
	commndList->SetGraphicsRootDescriptorTable(4, DrawListGPUSRV);

	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(RWDrawArgs.Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT);
	commndList->ResourceBarrier(1, &resourceBarrier);

	commndList->ExecuteIndirect(
		particleCommandSignature.Get(),
		1,
		RWDrawArgs.Get(),
		0,
		nullptr,
		0);

	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(RWDrawArgs.Get(),
		D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	commndList->ResourceBarrier(1, &resourceBarrier);

	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(RWDrawList.Get(),
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	commndList->ResourceBarrier(1, &resourceBarrier);

	resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(RWParticlePool.Get(),
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	commndList->ResourceBarrier(1, &resourceBarrier);
}

bool MeshGPUParticle::LoadMesh(const std::string modelname)
{
	std::ifstream file;

	const std::string filename = modelname + ".obj";
	const std::string directoryPath = "Resources/obj/" + modelname + "/";
	file.open(directoryPath + filename);

	assert(!file.fail());

	std::string line;
	while (getline(file, line)) 
	{
		std::istringstream line_stream(line);

		std::string key;
		std::getline(line_stream, key, ' ');

		if (key == "v") 
		{
			KMyMath::Vector3 pos{};
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;

			Vertex vertex{};
			vertex.position = pos;
			vertices_.emplace_back(vertex);
		}

		if (key == "vn" || key == "vt" || key == "f") 
		{
			break;
		}
	}
	file.close();

	return true;
}

void MeshGPUParticle::BuildUAV(Emitter* emitter)
{
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12Device* device = directXCommon->GetDevice();

	// Particle Pool
	{
		D3D12_DESCRIPTOR_HEAP_DESC uavHeapDesc = {};
		uavHeapDesc.NumDescriptors = 2048;
		uavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		uavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(device->CreateDescriptorHeap(&uavHeapDesc,
			IID_PPV_ARGS(&UAVHeap)));

		UINT64 particlePoolByteSize = 
			sizeof(Particle) * emitter->GetMaxParticles();
		CD3DX12_HEAP_PROPERTIES heap = 
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC resouceDesc =
			CD3DX12_RESOURCE_DESC::Buffer(particlePoolByteSize,
				D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
		ThrowIfFailed(device->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&resouceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&RWParticlePool)));
		directXCommon->Transition(RWParticlePool.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
		RWParticlePool->SetName(L"ParticlePool");

		D3D12_UNORDERED_ACCESS_VIEW_DESC particlePoolUAVDescription = {};
		particlePoolUAVDescription.Format = DXGI_FORMAT_UNKNOWN;
		particlePoolUAVDescription.Buffer.FirstElement = 0;
		particlePoolUAVDescription.Buffer.NumElements = emitter->GetMaxParticles();
		particlePoolUAVDescription.Buffer.StructureByteStride = sizeof(Particle);
		particlePoolUAVDescription.Buffer.CounterOffsetInBytes = 0;
		particlePoolUAVDescription.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;

		D3D12_SHADER_RESOURCE_VIEW_DESC particlePoolSRVDescription = {};
		particlePoolSRVDescription.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		particlePoolSRVDescription.Format = DXGI_FORMAT_UNKNOWN;
		particlePoolSRVDescription.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		particlePoolSRVDescription.Buffer.FirstElement = 0;
		particlePoolSRVDescription.Buffer.NumElements = emitter->GetMaxParticles();
		particlePoolSRVDescription.Buffer.StructureByteStride = sizeof(Particle);

		ParticlePoolCPUUAV =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(UAVHeap->GetCPUDescriptorHandleForHeapStart(), 0, directXCommon->GetCBVSRVUAVDescriptorSize());
		ParticlePoolGPUUAV =
			CD3DX12_GPU_DESCRIPTOR_HANDLE(UAVHeap->GetGPUDescriptorHandleForHeapStart(), 0, directXCommon->GetCBVSRVUAVDescriptorSize());
		device->CreateUnorderedAccessView(RWParticlePool.Get(), nullptr, &particlePoolUAVDescription, ParticlePoolCPUUAV);

		ParticlePoolCPUSRV =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(UAVHeap->GetCPUDescriptorHandleForHeapStart(), 4, directXCommon->GetCBVSRVUAVDescriptorSize());
		ParticlePoolGPUSRV =
			CD3DX12_GPU_DESCRIPTOR_HANDLE(UAVHeap->GetGPUDescriptorHandleForHeapStart(), 4, directXCommon->GetCBVSRVUAVDescriptorSize());
		device->CreateShaderResourceView(RWParticlePool.Get(), &particlePoolSRVDescription, ParticlePoolCPUSRV);
	}

	// Dead List
	{
		UINT64 deadListByteSize = sizeof(unsigned int) * emitter->GetMaxParticles();
		UINT64 countBufferOffset = AlignForUavCounter((UINT)deadListByteSize);

		CD3DX12_HEAP_PROPERTIES heap = 
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC resouceDesc =
			CD3DX12_RESOURCE_DESC::Buffer(countBufferOffset + sizeof(UINT),
				D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
		ThrowIfFailed(device->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&resouceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&ACDeadList)
		));
		ACDeadList->SetName(L"ACDeadList");

		D3D12_UNORDERED_ACCESS_VIEW_DESC deadListUAVDescription = {};
		deadListUAVDescription.Format = DXGI_FORMAT_UNKNOWN;
		deadListUAVDescription.Buffer.FirstElement = 0;
		deadListUAVDescription.Buffer.NumElements = emitter->GetMaxParticles();
		deadListUAVDescription.Buffer.StructureByteStride = sizeof(unsigned	int);
		deadListUAVDescription.Buffer.CounterOffsetInBytes = countBufferOffset;
		deadListUAVDescription.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;

		ACDeadListCPUUAV =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(UAVHeap->GetCPUDescriptorHandleForHeapStart(), 1, directXCommon->GetCBVSRVUAVDescriptorSize());
		ACDeadListGPUUAV =
			CD3DX12_GPU_DESCRIPTOR_HANDLE(UAVHeap->GetGPUDescriptorHandleForHeapStart(), 1, directXCommon->GetCBVSRVUAVDescriptorSize());
		device->CreateUnorderedAccessView(ACDeadList.Get(), ACDeadList.Get(), &deadListUAVDescription, ACDeadListCPUUAV);
	}

	// Draw List
	{
		UINT64 drawListByteSize = 
			sizeof(ParticleSort) * emitter->GetMaxParticles();
		UINT64 countBufferOffset = AlignForUavCounter((UINT)drawListByteSize);

		CD3DX12_HEAP_PROPERTIES heap = 
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC resouceDesc =
			CD3DX12_RESOURCE_DESC::Buffer(countBufferOffset + sizeof(UINT),
				D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
		ThrowIfFailed(device->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&resouceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&RWDrawList)
		));
		RWDrawList->SetName(L"DrawList");

		D3D12_UNORDERED_ACCESS_VIEW_DESC drawListUAVDescription = {};
		drawListUAVDescription.Format = DXGI_FORMAT_UNKNOWN;
		drawListUAVDescription.Buffer.FirstElement = 0;
		drawListUAVDescription.Buffer.NumElements = emitter->GetMaxParticles();
		drawListUAVDescription.Buffer.StructureByteStride = sizeof(ParticleSort);
		drawListUAVDescription.Buffer.CounterOffsetInBytes = countBufferOffset;
		drawListUAVDescription.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
		drawListUAVDescription.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;

		DrawListCPUUAV =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(UAVHeap->GetCPUDescriptorHandleForHeapStart(), 2, directXCommon->GetCBVSRVUAVDescriptorSize());
		DrawListGPUUAV =
			CD3DX12_GPU_DESCRIPTOR_HANDLE(UAVHeap->GetGPUDescriptorHandleForHeapStart(), 2, directXCommon->GetCBVSRVUAVDescriptorSize());
		device->CreateUnorderedAccessView(RWDrawList.Get(), RWDrawList.Get(), &drawListUAVDescription, DrawListCPUUAV);

		D3D12_SHADER_RESOURCE_VIEW_DESC drawListSRVDescription = {};
		drawListSRVDescription.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		drawListSRVDescription.Format = DXGI_FORMAT_UNKNOWN;
		drawListSRVDescription.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		drawListSRVDescription.Buffer.FirstElement = 0;
		drawListSRVDescription.Buffer.NumElements = emitter->GetMaxParticles();
		drawListSRVDescription.Buffer.StructureByteStride = sizeof(ParticleSort);

		DrawListCPUSRV =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(UAVHeap->GetCPUDescriptorHandleForHeapStart(), 5, directXCommon->GetCBVSRVUAVDescriptorSize());
		DrawListGPUSRV =
			CD3DX12_GPU_DESCRIPTOR_HANDLE(UAVHeap->GetGPUDescriptorHandleForHeapStart(), 5, directXCommon->GetCBVSRVUAVDescriptorSize());
		device->CreateShaderResourceView(RWDrawList.Get(), &drawListSRVDescription, DrawListCPUSRV);

		heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		resouceDesc = CD3DX12_RESOURCE_DESC::Buffer(countBufferOffset + sizeof(UINT));
		ThrowIfFailed(device->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&resouceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&DrawListUploadBuffer)
		));
	}

	// Draw Args
	{
		UINT64 drawArgsByteSize = (sizeof(unsigned int) * 9);
		UINT64 countBufferOffset = AlignForUavCounter((UINT)drawArgsByteSize);

		CD3DX12_HEAP_PROPERTIES heap = 
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC resouceDesc =
			CD3DX12_RESOURCE_DESC::Buffer(countBufferOffset + sizeof(UINT),
				D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
		ThrowIfFailed(device->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&resouceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&RWDrawArgs)
		));
		RWDrawArgs.Get()->SetName(L"DrawArgs");

		D3D12_UNORDERED_ACCESS_VIEW_DESC drawArgsUAVDescription = {};
		drawArgsUAVDescription.Format = DXGI_FORMAT_UNKNOWN;
		drawArgsUAVDescription.Buffer.FirstElement = 0;
		drawArgsUAVDescription.Buffer.NumElements = 9;
		drawArgsUAVDescription.Buffer.StructureByteStride = sizeof(unsigned int);
		drawArgsUAVDescription.Buffer.CounterOffsetInBytes = countBufferOffset;
		drawArgsUAVDescription.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
		drawArgsUAVDescription.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;

		DrawArgsCPUUAV =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(UAVHeap->GetCPUDescriptorHandleForHeapStart(), 3, directXCommon->GetCBVSRVUAVDescriptorSize());
		DrawArgsGPUUAV =
			CD3DX12_GPU_DESCRIPTOR_HANDLE(UAVHeap->GetGPUDescriptorHandleForHeapStart(), 3, directXCommon->GetCBVSRVUAVDescriptorSize());
		device->CreateUnorderedAccessView(RWDrawArgs.Get(), RWDrawArgs.Get(), &drawArgsUAVDescription, DrawArgsCPUUAV);
	}

	vertexs.reset(new KVertex(directXCommon->GetDevice(), vertices_));
	MeshSRV = vertexs->CreateDescripterSRV(UAVHeap.Get());
}

void MeshGPUParticle::BuildRootSignature()
{
	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDevice();

	// default root signature
	{
		rootSignature_->Add(RootType::CBV, 0);// b0
		rootSignature_->Add(RootType::CBV, 1);// b1
		rootSignature_->Add(RootType::CBV, 2);// b2
		rootSignature_->Add(RangeType::SRV, 0);
		rootSignature_->Add(RangeType::SRV, 1);

		rootSignature_->AddStaticSampler(0, 0, D3D12_FILTER_MIN_MAG_MIP_POINT);
		rootSignature_->AddStaticSampler(1, 0, D3D12_FILTER_MIN_MAG_MIP_POINT,
			AddressMode::Clamp, AddressMode::Clamp, AddressMode::Clamp);
		rootSignature_->AddStaticSampler(2);
		rootSignature_->AddStaticSampler(3, 0, D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			AddressMode::Clamp, AddressMode::Clamp, AddressMode::Clamp);
		rootSignature_->AddStaticSampler(4, 0, D3D12_FILTER_ANISOTROPIC);
		rootSignature_->AddStaticSampler(5, 0, D3D12_FILTER_ANISOTROPIC,
			AddressMode::Clamp, AddressMode::Clamp, AddressMode::Clamp);

		rootSignature_->Create(device);
	}

	// particle root signature
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

	// particle commnd signature
	D3D12_INDIRECT_ARGUMENT_DESC Args[1];
	Args[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;

	D3D12_COMMAND_SIGNATURE_DESC particleCommandSingatureDescription = {};
	particleCommandSingatureDescription.ByteStride = 36;
	particleCommandSingatureDescription.NumArgumentDescs = 1;
	particleCommandSingatureDescription.pArgumentDescs = Args;

	ThrowIfFailed(device->CreateCommandSignature(
		&particleCommandSingatureDescription,
		NULL,
		IID_PPV_ARGS(particleCommandSignature.GetAddressOf())));
}

void MeshGPUParticle::BuildShadersAndInputLayout()
{
	
}

void MeshGPUParticle::BuildPSOs()
{
	//KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDevice();

	// Mesh
	{
		// 頂点データを準備
		std::vector<Vertex> vertexData;
		for (size_t i = 0; i < vertices_.size(); ++i) {
			vertexData.push_back({ vertices_[i] });
		}

		// バッファリソースを作成
		D3D12_HEAP_PROPERTIES heapProps = {};
		heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

		D3D12_RESOURCE_DESC bufferDesc = {};
		bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		bufferDesc.Width = sizeof(Vertex) * vertexData.size();
		bufferDesc.Height = 1;
		bufferDesc.DepthOrArraySize = 1;
		bufferDesc.MipLevels = 1;
		bufferDesc.SampleDesc.Count = 1;
		bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		ID3D12Resource* vertexBuffer;
		device->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&bufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertexBuffer)
		);

		// バッファにデータを書き込む
		void* mappedData = nullptr;
		vertexBuffer->Map(0, nullptr, &mappedData);
		memcpy(mappedData, vertexData.data(), sizeof(Vertex) * vertexData.size());
		vertexBuffer->Unmap(0, nullptr);
	}

	graphicPSO_->SetRootSignature(rootSignature_->GetRootSignature());
	graphicPSO_->CreateVertexShader(L"MeshGPUParticle/MeshGPUParticleVS.hlsl", "main");
	graphicPSO_->CreatePixelShader(L"MeshGPUParticle/MeshGPUParticlePS.hlsl", "main");
	graphicPSO_->CreateGeometryShader(L"MeshGPUParticle/MeshGPUParticleGS.hlsl", "main");

	D3D12_BLEND_DESC lBlendDesc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	lBlendDesc = CreateBlend(BlendMode::ADD);

	RenderTargetFormat renderTargetFormat{};
	renderTargetFormat.NumRenderTargets = 1;
	renderTargetFormat.RTVFormats[0] = BackBufferFormat;

	graphicPSO_->SetPrimitiveType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT);
	graphicPSO_->SetRenderTargetFormat(renderTargetFormat);
	graphicPSO_->SetDepthFlag(false);
	graphicPSO_->SetDepthWriteMask(D3D12_DEPTH_WRITE_MASK_ZERO);
	graphicPSO_->SetFillMode(D3D12_FILL_MODE_SOLID);
	graphicPSO_->SetBlend(lBlendDesc);
	graphicPSO_->Create(device);
	// EmitCS
	emitPSO_->CreateShader(L"MeshGPUParticle/MeshEmitCS.hlsl", "main");
	emitPSO_->SetRootSignature(particleRootSignature_.get());
	emitPSO_->SetFlag(D3D12_PIPELINE_STATE_FLAG_NONE);
	emitPSO_->Create(device);

	// UpdateCS
	updatePSO_->CreateShader(L"MeshGPUParticle/MeshUpdateCS.hlsl", "main");
	updatePSO_->SetRootSignature(particleRootSignature_.get());
	updatePSO_->SetFlag(D3D12_PIPELINE_STATE_FLAG_NONE);
	updatePSO_->Create(device);

	// CopyDrawCountCS
	copyDrawPSO_->CreateShader(L"MeshGPUParticle/MeshCopyDrawCountCS.hlsl", "main");
	copyDrawPSO_->SetRootSignature(particleRootSignature_.get());
	copyDrawPSO_->SetFlag(D3D12_PIPELINE_STATE_FLAG_NONE);
	copyDrawPSO_->Create(device);

	// DeadListInitCS
	deadListPSO_->CreateShader(L"MeshGPUParticle/MeshDeadListInitCS.hlsl", "main");
	deadListPSO_->SetRootSignature(particleRootSignature_.get());
	deadListPSO_->SetFlag(D3D12_PIPELINE_STATE_FLAG_NONE);
	deadListPSO_->Create(device);
}

void MeshGPUParticle::BuildFrameResources()
{
	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDevice();

	for (int i = 0; i < gNumberFrameResources; ++i)
	{
		FrameResources.push_back(std::make_unique<FrameResource>(device, 1, 1, 1));
	}
}

void MeshGPUParticle::UpdateMainPassCB(const Timer& timer, const KMyMath::Matrix4& matView, const KMyMath::Matrix4& matProjection, Emitter* emitter)
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
	MainParticleCB.vertexNum = (uint32_t)vertices_.size();

	auto currentParticleCB = currentFrameResource->ParticleCB.get();
	currentParticleCB->CopyData(0, MainParticleCB);
}

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> MeshGPUParticle::GetStaticSamplers()
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
