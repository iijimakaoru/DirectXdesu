#pragma once
#include <wrl.h>

#include "Timer.h"
#include "Emitter.h"
#include "FrameResource.h"
#include "KVertex.h"

#include "RootSignature.h"
#include "ComputePipelineState.h"
#include "GraphicPipelineState.h"

#include "ParticlePool.h"
#include "DeadList.h"
#include "DrawList.h"
#include "DrawArgs.h"

#include "CommandSignature.h"

#include "KModel.h"

class MeshGPUParticle
{
public:
	MeshGPUParticle(const Timer& timer,
		const KMyMath::Matrix4& matView,
		const KMyMath::Matrix4& matProjection,
		Emitter* emitter,
		const std::string modelname = "test");

	void Init(const Timer& timer,
		const KMyMath::Matrix4& matView,
		const KMyMath::Matrix4& matProjection,
		Emitter* emitter,
		const std::string modelname);

	void Update(const Timer& timer,
		const KMyMath::Matrix4& matView,
		const KMyMath::Matrix4& matProjection,
		Emitter* emitter);

	void Draw(const Timer& timer,
		const KMyMath::Matrix4& matView,
		const KMyMath::Matrix4& matProjection,
		Emitter* emitter);

private:
	void BuildUAV();
	void BuildRootSignature();
	void BuildPSOs();
	void BuildFrameResources();
	void UpdateMainPassCB(const Timer& timer,
		const KMyMath::Matrix4& matView,
		const KMyMath::Matrix4& matProjection,
		Emitter* emitter);
	void ParticleUpdate();
	void ParticleDraw();
	void DrawCommon();

private:
	DXGI_FORMAT BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	std::vector<std::unique_ptr<FrameResource>> FrameResources;
	FrameResource* currentFrameResource = nullptr;
	int currentFrameResourceIndex = 0;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> UAVHeap = nullptr;

	ObjectConstants MainObjectCB;
	TimeConstants MainTimeCB;
	ParticleConstants MainParticleCB;

	std::unique_ptr<RootSignature> rootSignature_;
	std::unique_ptr<RootSignature> particleRootSignature_;

	std::unique_ptr<GraphicPipelineState> graphicPSO_;

	std::unique_ptr<ComputePipelineState> emitPSO_;
	std::unique_ptr<ComputePipelineState> updatePSO_;
	std::unique_ptr<ComputePipelineState> copyDrawPSO_;
	std::unique_ptr<ComputePipelineState> deadListPSO_;

	std::unique_ptr<ParticlePool> particlePool_;
	std::unique_ptr<DeadList> deadList_;
	std::unique_ptr<DrawList> drawList_;
	std::unique_ptr<DrawArgs> drawArgs_;

	std::unique_ptr<CommandSignature> commandSignature_;

	std::unique_ptr<MeshModel> meshModel_;
	CD3DX12_GPU_DESCRIPTOR_HANDLE MeshSRV;

	bool init = false;
};

