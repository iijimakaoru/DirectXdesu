#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

#include "Emitter.h"

class DrawList
{
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> RWDrawList = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> DrawListUploadBuffer = nullptr;

	CD3DX12_CPU_DESCRIPTOR_HANDLE DrawListCPUSRV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE DrawListGPUSRV;

	CD3DX12_CPU_DESCRIPTOR_HANDLE DrawListCPUUAV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE DrawListGPUUAV;

public:
	void Create(ID3D12DescriptorHeap* uavHeap, uint32_t particleMax);
	ID3D12Resource* GetDrawList();
	ID3D12Resource* GetDrawListUploadBuffer();
	CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCPUSRV();
	CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGPUSRV();
	CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCPUUAV();
	CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGPUUAV();

	// We pack the UAV counter into the same buffer as the commands rather than create
	// a separate 64K resource/heap for it. The counter must be aligned on 4K boundaries,
	// so we pad the command buffer (if necessary) such that the counter will be placed
	// at a valid location in the buffer.
	static inline UINT AlignForUavCounter(UINT bufferSize)
	{
		const UINT alignment = D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT;
		return (bufferSize + (alignment - 1)) & ~(alignment - 1);
	}
};

