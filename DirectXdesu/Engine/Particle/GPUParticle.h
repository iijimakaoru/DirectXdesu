#pragma once
#include <wrl.h>

#include "Timer.h"
#include "Emitter.h"
#include "FrameResource.h"

class GPUParticle
{
public:
	GPUParticle(const Timer& timer,
		const KMyMath::Matrix4& matView,
		const KMyMath::Matrix4& matProjection,
		Emitter* emitter);

	void Init(const Timer& timer,
		const KMyMath::Matrix4& matView,
		const KMyMath::Matrix4& matProjection,
		Emitter* emitter);

	void Update(const Timer& timer,
		const KMyMath::Matrix4& matView,
		const KMyMath::Matrix4& matProjection,
		Emitter* emitter);

	void Draw(const Timer& timer,
		const KMyMath::Matrix4& matView,
		const KMyMath::Matrix4& matProjection,
		Emitter* emitter);

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

	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();

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

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> particleRootSignature = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandSignature> particleCommandSignature = nullptr;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> UAVHeap = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> RWParticlePool = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> ACDeadList = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> RWDrawList = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> RWDrawArgs = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> DrawListUploadBuffer = nullptr;

	CD3DX12_CPU_DESCRIPTOR_HANDLE ParticlePoolCPUSRV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE ParticlePoolGPUSRV;

	CD3DX12_CPU_DESCRIPTOR_HANDLE ParticlePoolCPUUAV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE ParticlePoolGPUUAV;

	CD3DX12_CPU_DESCRIPTOR_HANDLE ACDeadListCPUUAV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE ACDeadListGPUUAV;

	CD3DX12_CPU_DESCRIPTOR_HANDLE DrawListCPUSRV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE DrawListGPUSRV;

	CD3DX12_CPU_DESCRIPTOR_HANDLE DrawListCPUUAV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE DrawListGPUUAV;

	CD3DX12_CPU_DESCRIPTOR_HANDLE DrawArgsCPUUAV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE DrawArgsGPUUAV;

	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3DBlob>> Shaders;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D12PipelineState>> PSOs;

	ObjectConstants MainObjectCB;
	TimeConstants MainTimeCB;
	ParticleConstants MainParticleCB;
};

