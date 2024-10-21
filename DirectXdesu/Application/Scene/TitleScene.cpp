#include "TitleScene.h"
#include "FbxLoader.h"

#include "DebugCamera.h"

#include "SceneManager.h"

#include <imgui.h>

#include "Ease.h"

#include "ModelManager.h"
#include "PipelineManager.h"

#include "PostEffectManager.h"

TitleScene::~TitleScene() { Final(); }

void TitleScene::LoadResources() {
	// 機体モデル
	model = ModelManager::GetInstance()->GetModels("Player");

	// 天球モデル
	skyDomeModel = ModelManager::GetInstance()->GetModels("T_SkyDorm");
}

void TitleScene::Init() {
	//ID3D12Device* device = KDirectXCommon::GetInstance()->GetDev();

	BaseScene::Init();

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

	camera->StartRound();
}

void TitleScene::Update() {
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
	ID3D12Device* device = directXCommon->GetDev();

	// Particle Pool
	{
		D3D12_DESCRIPTOR_HEAP_DESC uavHeapDesc = {};
		uavHeapDesc.NumDescriptors = 6;
		uavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		uavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(device->CreateDescriptorHeap(&uavHeapDesc, IID_PPV_ARGS(&UAVHeap)));

		UINT64 particlePoolByteSize = sizeof(Particle) * emitter->GetMaxParticles();
		CD3DX12_HEAP_PROPERTIES heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC resouceDesc = CD3DX12_RESOURCE_DESC::Buffer(particlePoolByteSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
		ThrowIfFailed(device->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&resouceDesc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			nullptr,
			IID_PPV_ARGS(&RWParticlePool)));
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

		ParticlePoolCPUUAV = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			UAVHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			directXCommon->GetCBVSRVUAVDescriptorSize());
		ParticlePoolGPUUAV = 
			CD3DX12_GPU_DESCRIPTOR_HANDLE(
				UAVHeap->GetGPUDescriptorHandleForHeapStart(),
				0, 
				directXCommon->GetCBVSRVUAVDescriptorSize());
		device->CreateUnorderedAccessView(RWParticlePool.Get(),
			nullptr, &particlePoolUAVDescription, ParticlePoolCPUUAV);

		ParticlePoolCPUSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			UAVHeap->GetCPUDescriptorHandleForHeapStart(),
			4,
			directXCommon->GetCBVSRVUAVDescriptorSize());
		ParticlePoolGPUSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(
			UAVHeap->GetGPUDescriptorHandleForHeapStart(),
			4,
			directXCommon->GetCBVSRVUAVDescriptorSize());
		device->CreateShaderResourceView(RWParticlePool.Get(),
			&particlePoolSRVDescription, ParticlePoolCPUUAV);
	}

	// Dead List
	{
		UINT64 deadListByteSize = sizeof(unsigned int) * emitter->GetMaxParticles();
		UINT64 countBufferOffset = AlignForUavCounter((UINT)deadListByteSize);

		CD3DX12_HEAP_PROPERTIES heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC resouceDesc = 
			CD3DX12_RESOURCE_DESC::Buffer(countBufferOffset + sizeof(UINT), 
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
		ThrowIfFailed(device->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&resouceDesc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			nullptr,
			IID_PPV_ARGS(&ACDeadList)
		));
		ACDeadList->SetName(L"ACDeadList");

		D3D12_UNORDERED_ACCESS_VIEW_DESC drawlistUAVDescription = {};
		drawlistUAVDescription.Format = DXGI_FORMAT_UNKNOWN;
		drawlistUAVDescription.Buffer.FirstElement = 0;
		drawlistUAVDescription.Buffer.NumElements = emitter->GetMaxParticles();
		drawlistUAVDescription.Buffer.StructureByteStride = sizeof(Particle);
		drawlistUAVDescription.Buffer.CounterOffsetInBytes = countBufferOffset;
		drawlistUAVDescription.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
		drawlistUAVDescription.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;

		DrawListCPUUAV = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			UAVHeap->GetCPUDescriptorHandleForHeapStart(),
			2,
			directXCommon->GetCBVSRVUAVDescriptorSize());
		DrawListGPUUAV = CD3DX12_GPU_DESCRIPTOR_HANDLE(
			UAVHeap->GetGPUDescriptorHandleForHeapStart(),
			2,
			directXCommon->GetCBVSRVUAVDescriptorSize());
		device->CreateUnorderedAccessView(RWDrawList.Get(), RWDrawList.Get(), &drawlistUAVDescription, DrawListCPUUAV);

		D3D12_SHADER_RESOURCE_VIEW_DESC drawListSRVDescription = {};
		drawListSRVDescription.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		drawListSRVDescription.Format = DXGI_FORMAT_UNKNOWN;
		drawListSRVDescription.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		drawListSRVDescription.Buffer.FirstElement = 0;
		drawListSRVDescription.Buffer.NumElements = emitter->GetMaxParticles();
		drawListSRVDescription.Buffer.StructureByteStride = sizeof(ParticleSort);

		DrawListCPUSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			UAVHeap->GetCPUDescriptorHandleForHeapStart(),
			5,
			directXCommon->GetCBVSRVUAVDescriptorSize());
		DrawListGPUSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(
			UAVHeap->GetGPUDescriptorHandleForHeapStart(),
			5,
			directXCommon->GetCBVSRVUAVDescriptorSize());
		device->CreateShaderResourceView(RWDrawList.Get(), &drawListSRVDescription, DrawListCPUSRV);

		heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		resouceDesc = CD3DX12_RESOURCE_DESC::Buffer(countBufferOffset + sizeof(UINT));
		device->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&resouceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&DrawListUploadBuffer)
		);
	}

	// Draw Args
	{
		UINT64 drawArgsByteSize = (sizeof(unsigned int) * 9);
		UINT64 countBufferOffset = AlignForUavCounter((UINT)drawArgsByteSize);

		CD3DX12_HEAP_PROPERTIES heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC resouceDesc = CD3DX12_RESOURCE_DESC::Buffer(countBufferOffset + sizeof(UINT),
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
		ThrowIfFailed(device->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&resouceDesc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
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

		DrawArgsCPUUAV = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			UAVHeap->GetCPUDescriptorHandleForHeapStart(),
			3,
			directXCommon->GetCBVSRVUAVDescriptorSize());
		DrawArgsGPUUAV = CD3DX12_GPU_DESCRIPTOR_HANDLE(
			UAVHeap->GetGPUDescriptorHandleForHeapStart(),
			3,
			directXCommon->GetCBVSRVUAVDescriptorSize());
		device->CreateUnorderedAccessView(RWDrawArgs.Get(), RWDrawArgs.Get(), &drawArgsUAVDescription, DrawArgsCPUUAV);
	}
}
