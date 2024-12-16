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

	bool LoadMesh(const std::string modelname);

private:
	void BuildUAV(Emitter* emitter);
	void BuildRootSignature();
	void BuildShadersAndInputLayout();
	void BuildPSOs();
	void BuildFrameResources();
	void UpdateMainPassCB(const Timer& timer,
		const KMyMath::Matrix4& matView,
		const KMyMath::Matrix4& matProjection,
		Emitter* emitter);

	// We pack the UAV counter into the same buffer as the commands rather than create
	// a separate 64K resource/heap for it. The counter must be aligned on 4K boundaries,
	// so we pad the command buffer (if necessary) such that the counter will be placed
	// at a valid location in the buffer.
	static inline UINT AlignForUavCounter(UINT bufferSize)
	{
		const UINT alignment = D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT;
		return (bufferSize + (alignment - 1)) & ~(alignment - 1);
	}

private:
	DXGI_FORMAT BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	std::vector<std::unique_ptr<FrameResource>> FrameResources;
	FrameResource* currentFrameResource = nullptr;
	int currentFrameResourceIndex = 0;

	Microsoft::WRL::ComPtr<ID3D12CommandSignature> particleCommandSignature = nullptr;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> UAVHeap = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> RWDrawArgs = nullptr;

	CD3DX12_CPU_DESCRIPTOR_HANDLE DrawArgsCPUUAV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE DrawArgsGPUUAV;

	CD3DX12_GPU_DESCRIPTOR_HANDLE MeshSRV;

	ObjectConstants MainObjectCB;
	TimeConstants MainTimeCB;
	ParticleConstants MainParticleCB;

	std::vector<Vertex> vertices_;
	std::unique_ptr<KVertex> vertexs;

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

	bool init = false;
};

