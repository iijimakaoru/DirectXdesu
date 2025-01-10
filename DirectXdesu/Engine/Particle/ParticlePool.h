#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include "Emitter.h"

class ParticlePool
{
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> RWParticlePool = nullptr;

	CD3DX12_CPU_DESCRIPTOR_HANDLE ParticlePoolCPUSRV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE ParticlePoolGPUSRV;

	CD3DX12_CPU_DESCRIPTOR_HANDLE ParticlePoolCPUUAV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE ParticlePoolGPUUAV;

	D3D12_RESOURCE_STATES resourseState;

public:
	void Create(ID3D12DescriptorHeap* uavHeap, uint32_t particleMax);
	ID3D12Resource* GetParticlePool();
	CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCPUSRV();
	CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGPUSRV();
	CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCPUUAV();
	CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGPUUAV();
	void Translation(ID3D12GraphicsCommandList* cmdList, D3D12_RESOURCE_STATES afterState);
};

