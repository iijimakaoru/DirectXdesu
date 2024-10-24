#include "TitleScene.h"
#include "FbxLoader.h"

#include "DebugCamera.h"

#include "SceneManager.h"

#include <imgui.h>

#include "Ease.h"

#include "ModelManager.h"
#include "PipelineManager.h"

#include "PostEffectManager.h"

const int gNumberFrameResources = 3;

TitleScene::~TitleScene() { Final(); }

void TitleScene::LoadResources() {
	// 機体モデル
	model = ModelManager::GetInstance()->GetModels("Player");

	// 天球モデル
	skyDomeModel = ModelManager::GetInstance()->GetModels("T_SkyDorm");
}

void TitleScene::Init() {
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	//ID3D12Device* device = KDirectXCommon::GetInstance()->GetDev();

	timer_ = Timer(KWinApp::GetHWND(), KWinApp::GetWindow().lpszMenuName);

	BaseScene::Init();

	KDirectXCommon::GetInstance()->CloseCommnd();

	// reset the command list to prep for initialization commands
	ThrowIfFailed(directXCommon->GetCommandList()->Reset(
		directXCommon->GetCommandAllocator().Get(), nullptr));

	// インスタンス
	input = KInput::GetInstance();

	light_.reset(Light::Create());
	light_->SetLightRGB({1.0f, 1.0f, 1.0f});
	light_->SetLightDir({0, -1, 0, 0.0f});
	KObject3d::SetLight(light_.get());

	// カメラ読み込み
	camera = std::make_unique<TitleCamera>();
	camera->Init();

	sceneManager = SceneManager::GetInstance();

	// オブジェクト生成
	object3d.reset(KObject3d::Create(model, PipelineManager::GetInstance()->GetPipeline("Obj")));
	object3d->GetTransform().SetScale({0.0f, 0.0f, 0.0f});

	skyDome.reset(
	    KObject3d::Create(skyDomeModel, PipelineManager::GetInstance()->GetPipeline("Obj")));
	skyDome->GetTransform().SetScale({200.0f, 200.0f, 200.0f});

	audioManager = AudioManager::GetInstance();

	emitter = new Emitter(
		10000,
		100,
		10000.0f,
		120.0f,
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)
	);

	BuildUAV();

	KDirectXCommon::GetInstance()->CloseCommnd();

	KDirectXCommon::GetInstance()->BeginCommnd();

	BuildRootSignature();
	BuildShadersAndInputLayout();
	BuildFrameResources();
	BuildPSOs();

	KDirectXCommon::GetInstance()->CloseCommnd();

	ThrowIfFailed(directXCommon->GetCommandAllocator()->Reset());

	ThrowIfFailed(
		directXCommon->GetCommandList()->Reset(directXCommon->GetCommandAllocator().Get(),
			PSOs["particleDeadList"].Get()));

	directXCommon->GetCommandList()->SetComputeRootSignature(particleRootSignature.Get());

	currentFrameResourceIndex = (currentFrameResourceIndex + 1) % gNumberFrameResources;
	currentFrameResource = FrameResources[currentFrameResourceIndex].get();

	UpdateMainPassCB(timer_);

	camera->StartRound();
}

void TitleScene::Update() {
	timer_.UpdateTimer();

	light_->SetLightRGB({lightRGB.x, lightRGB.y, lightRGB.z});
	light_->SetLightDir({lightDir.x, lightDir.y, lightDir.z, 0.0f});

	light_->Update();

	object3d->Update(camera->GetViewPro(), camera->GetWorldPos());

	skyDome->Update(camera->GetViewPro(), camera->GetWorldPos());

	camera->Update();
}

void TitleScene::ObjDraw() {
	object3d->Draw();

	skyDome->Draw();
}

void TitleScene::SpriteDraw() {
	
}

void TitleScene::Final() { delete emitter; }

void TitleScene::StartScene() {
	
}

void TitleScene::GoNextScene() {
	
}

void TitleScene::TitleCall() {
	
}

void TitleScene::BuildUAV()
{
	KDirectXCommon* directXCommon = KDirectXCommon::GetInstance();
	ID3D12Device* device = directXCommon->GetDevice();

	// Particle Pool
	{
		D3D12_DESCRIPTOR_HEAP_DESC uavHeapDesc = {};
		uavHeapDesc.NumDescriptors = 6;
		uavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		uavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(device->CreateDescriptorHeap(&uavHeapDesc, IID_PPV_ARGS(&UAVHeap)));

		UINT64 particlePoolByteSize = sizeof(Particle) * emitter->GetMaxParticles();
		CD3DX12_HEAP_PROPERTIES heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
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
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
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
		device->CreateShaderResourceView(RWParticlePool.Get(), &particlePoolSRVDescription, ParticlePoolCPUUAV);
	}

	// Dead List
	{
		UINT64 deadListByteSize = sizeof(unsigned int) * emitter->GetMaxParticles();
		UINT64 countBufferOffset = AlignForUavCounter((UINT)deadListByteSize);

		CD3DX12_HEAP_PROPERTIES heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC resouceDesc =
			CD3DX12_RESOURCE_DESC::Buffer(countBufferOffset + sizeof(UINT), D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
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
		UINT64 drawListByteSize = sizeof(ParticleSort) * emitter->GetMaxParticles();
		UINT64 countBufferOffset = AlignForUavCounter((UINT)drawListByteSize);

		CD3DX12_HEAP_PROPERTIES heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC resouceDesc =
			CD3DX12_RESOURCE_DESC::Buffer(countBufferOffset + sizeof(UINT), D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
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

		CD3DX12_HEAP_PROPERTIES heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC resouceDesc = 
			CD3DX12_RESOURCE_DESC::Buffer(countBufferOffset + sizeof(UINT),D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
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
			CD3DX12_CPU_DESCRIPTOR_HANDLE(UAVHeap->GetCPUDescriptorHandleForHeapStart(),3,directXCommon->GetCBVSRVUAVDescriptorSize());
		DrawArgsGPUUAV = 
			CD3DX12_GPU_DESCRIPTOR_HANDLE(UAVHeap->GetGPUDescriptorHandleForHeapStart(),3,directXCommon->GetCBVSRVUAVDescriptorSize());
		device->CreateUnorderedAccessView(RWDrawArgs.Get(), RWDrawArgs.Get(), &drawArgsUAVDescription, DrawArgsCPUUAV);
	}
}

void TitleScene::BuildRootSignature()
{
	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDevice();

	// default root signature
	{
		CD3DX12_DESCRIPTOR_RANGE srvTable0;
		srvTable0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

		CD3DX12_DESCRIPTOR_RANGE srvTable1;
		srvTable1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

		CD3DX12_ROOT_PARAMETER slotRootParameter[5];

		slotRootParameter[0].InitAsConstantBufferView(0);
		slotRootParameter[1].InitAsConstantBufferView(1);
		slotRootParameter[2].InitAsConstantBufferView(2);
		slotRootParameter[3].InitAsDescriptorTable(1, &srvTable0);
		slotRootParameter[4].InitAsDescriptorTable(1, &srvTable1);

		auto staticSamplers = GetStaticSamplers();

		CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(5, slotRootParameter,
			(UINT)staticSamplers.size(),
			staticSamplers.data(),
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		Microsoft::WRL::ComPtr<ID3D10Blob> serializedRootSig = nullptr;
		Microsoft::WRL::ComPtr<ID3D10Blob> errorBlob = nullptr;
		HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
			serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

		if (errorBlob != nullptr) 
		{
			::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}
		ThrowIfFailed(hr);

		ThrowIfFailed(device->CreateRootSignature(
			0,
			serializedRootSig->GetBufferPointer(),
			serializedRootSig->GetBufferSize(),
			IID_PPV_ARGS(rootSignature.GetAddressOf())
		));
	}

	// particle root signature
	{
		CD3DX12_DESCRIPTOR_RANGE uavTable0;
		uavTable0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);

		CD3DX12_DESCRIPTOR_RANGE uavTable1;
		uavTable1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1);

		CD3DX12_DESCRIPTOR_RANGE uavTable2;
		uavTable2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 2);

		CD3DX12_DESCRIPTOR_RANGE uavTable3;
		uavTable3.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 3);

		// Root parameter can be a table, root descriptor or root constants.
		CD3DX12_ROOT_PARAMETER slotRootParameter[7];

		// Perfomance TIP: Order from most frequent to least frequent.
		slotRootParameter[0].InitAsConstantBufferView(0);
		slotRootParameter[1].InitAsConstantBufferView(1);
		slotRootParameter[2].InitAsConstantBufferView(2);
		slotRootParameter[3].InitAsDescriptorTable(1, &uavTable0);
		slotRootParameter[4].InitAsDescriptorTable(1, &uavTable1);
		slotRootParameter[5].InitAsDescriptorTable(1, &uavTable2);
		slotRootParameter[6].InitAsDescriptorTable(1, &uavTable3);

		// A root signature is an array of root parameters.
		CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(7, slotRootParameter,
			0, nullptr,
			D3D12_ROOT_SIGNATURE_FLAG_NONE);

		// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
		Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
		HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
			serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

		if (errorBlob != nullptr)
		{
			::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}
		ThrowIfFailed(hr);

		ThrowIfFailed(device->CreateRootSignature(
			0,
			serializedRootSig->GetBufferPointer(),
			serializedRootSig->GetBufferSize(),
			IID_PPV_ARGS(particleRootSignature.GetAddressOf())));
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

void TitleScene::BuildShadersAndInputLayout()
{
	Shaders["VS"] = d3dUtil::CompileShader(L"Resources/Shader/GPUParticle/GPUParticleVS.hlsl",
		nullptr,"main","vs_5_0");
	Shaders["GS"] = d3dUtil::CompileShader(L"Resources/Shader/GPUParticle/GPUParticleGS.hlsl",
		nullptr, "main", "gs_5_0");
	Shaders["PS"] = d3dUtil::CompileShader(L"Resources/Shader/GPUParticle/GPUParticlePS.hlsl",
		nullptr, "main", "ps_5_0");
	Shaders["EmitCS"] = d3dUtil::CompileShader(L"Resources/Shader/GPUParticle/EmitCS.hlsl",
		nullptr, "main", "cs_5_0");
	Shaders["UpdateCS"] = d3dUtil::CompileShader(L"Resources/Shader/GPUParticle/UpdateCS.hlsl",
		nullptr, "main", "cs_5_0");
	Shaders["CopyDrawCountCS"] = d3dUtil::CompileShader(L"Resources/Shader/GPUParticle/CopyDrawCountCS.hlsl",
		nullptr, "main", "cs_5_0");
	Shaders["DeadListInitCS"] = d3dUtil::CompileShader(L"Resources/Shader/GPUParticle/DeadListInitCS.hlsl",
		nullptr, "main", "cs_5_0");
}

void TitleScene::BuildPSOs()
{
	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDevice();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC opaquePSODescription;
	ZeroMemory(&opaquePSODescription, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	opaquePSODescription.pRootSignature = rootSignature.Get();
	opaquePSODescription.VS =
	{
		reinterpret_cast<BYTE*>(Shaders["VS"]->GetBufferPointer()),
		Shaders["VS"]->GetBufferSize()
	};
	opaquePSODescription.PS =
	{
		reinterpret_cast<BYTE*>(Shaders["PS"]->GetBufferPointer()),
		Shaders["PS"]->GetBufferSize()
	};
	opaquePSODescription.GS =
	{
		reinterpret_cast<BYTE*>(Shaders["GS"]->GetBufferPointer()),
		Shaders["GS"]->GetBufferSize()
	};

	D3D12_RENDER_TARGET_BLEND_DESC transparencyBlendDesc = {};
	transparencyBlendDesc.BlendEnable = true;
	transparencyBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	transparencyBlendDesc.SrcBlend = D3D12_BLEND_ONE;
	transparencyBlendDesc.DestBlend = D3D12_BLEND_ONE;
	transparencyBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	transparencyBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	transparencyBlendDesc.DestBlendAlpha = D3D12_BLEND_ONE;
	transparencyBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_DEPTH_STENCIL_DESC depth = {};
	depth.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depth.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	depth.DepthEnable = true;

	opaquePSODescription.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	opaquePSODescription.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	opaquePSODescription.BlendState.RenderTarget[0] = transparencyBlendDesc;
	opaquePSODescription.DepthStencilState = depth;
	opaquePSODescription.SampleMask = UINT_MAX;
	opaquePSODescription.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	opaquePSODescription.NumRenderTargets = 1;
	opaquePSODescription.RTVFormats[0] = BackBufferFormat;
	opaquePSODescription.SampleDesc.Count = xMsaaState ? 4 : 1;
	opaquePSODescription.SampleDesc.Quality = xMsaaState ? (xMsaaQuality - 1) : 0;
	opaquePSODescription.DSVFormat = DepthStencilFormat;
	ThrowIfFailed(device->CreateGraphicsPipelineState(&opaquePSODescription, IID_PPV_ARGS(&PSOs["opaque"])));

	D3D12_COMPUTE_PIPELINE_STATE_DESC particleEmitPSO = {};
	particleEmitPSO.pRootSignature = particleRootSignature.Get();
	particleEmitPSO.CS =
	{
		reinterpret_cast<BYTE*>(Shaders["EmitCS"]->GetBufferPointer()),
		Shaders["EmitCS"]->GetBufferSize()
	};
	particleEmitPSO.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	ThrowIfFailed(device->CreateComputePipelineState(&particleEmitPSO, IID_PPV_ARGS(&PSOs["particleEmit"])));

	D3D12_COMPUTE_PIPELINE_STATE_DESC particleUpdatePSO = {};
	particleUpdatePSO.pRootSignature = particleRootSignature.Get();
	particleUpdatePSO.CS =
	{
		reinterpret_cast<BYTE*>(Shaders["UpdateCS"]->GetBufferPointer()),
		Shaders["UpdateCS"]->GetBufferSize()
	};
	particleUpdatePSO.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	ThrowIfFailed(device->CreateComputePipelineState(&particleUpdatePSO, IID_PPV_ARGS(&PSOs["particleUpdate"])));

	D3D12_COMPUTE_PIPELINE_STATE_DESC particleDrawPSO = {};
	particleDrawPSO.pRootSignature = particleRootSignature.Get();
	particleDrawPSO.CS =
	{
		reinterpret_cast<BYTE*>(Shaders["CopyDrawCountCS"]->GetBufferPointer()),
		Shaders["CopyDrawCountCS"]->GetBufferSize()
	};
	particleDrawPSO.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	ThrowIfFailed(device->CreateComputePipelineState(&particleDrawPSO, IID_PPV_ARGS(&PSOs["particleDraw"])));

	D3D12_COMPUTE_PIPELINE_STATE_DESC particleDeadListPSO = {};
	particleDeadListPSO.pRootSignature = particleRootSignature.Get();
	particleDeadListPSO.CS =
	{
		reinterpret_cast<BYTE*>(Shaders["DeadListInitCS"]->GetBufferPointer()),
		Shaders["DeadListInitCS"]->GetBufferSize()
	};
	particleDeadListPSO.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	ThrowIfFailed(device->CreateComputePipelineState(&particleDeadListPSO, IID_PPV_ARGS(&PSOs["particleDeadList"])));
}

void TitleScene::BuildFrameResources()
{
	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDevice();

	for (int i = 0; i < gNumberFrameResources; ++i)
	{
		FrameResources.push_back(std::make_unique<FrameResource>(device, 1, 1, 1));
	}
}

void TitleScene::UpdateMainPassCB(const Timer& timer)
{
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX view = MyMathConvert::ChangeMatrix4toXMMATRIX(camera->GetViewPro()->GetMatView());
	DirectX::XMMATRIX projection = MyMathConvert::ChangeMatrix4toXMMATRIX(camera->GetViewPro()->GetMatPro());

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

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> TitleScene::GetStaticSamplers()
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
