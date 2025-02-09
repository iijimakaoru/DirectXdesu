#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

#include "Emitter.h"

class DrawList
{
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> RWDrawList = nullptr;

	CD3DX12_CPU_DESCRIPTOR_HANDLE DrawListCPUSRV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE DrawListGPUSRV;

	CD3DX12_CPU_DESCRIPTOR_HANDLE DrawListCPUUAV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE DrawListGPUUAV;

	D3D12_RESOURCE_STATES resourseState;

public:
	void Create(ID3D12DescriptorHeap* uavHeap, uint32_t particleMax);
	ID3D12Resource* GetDrawList();
	CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCPUSRV();
	CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGPUSRV();
	CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCPUUAV();
	CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGPUUAV();
	void Translation(ID3D12GraphicsCommandList* cmdList, D3D12_RESOURCE_STATES afterState);

	// UAV カウンターを作成するのではなく、コマンドと同じバッファーにパックします。
	// それには別の 64K リソース/ヒープが必要です。カウンタは 4K 境界上に揃える必要があります。
	// そのため、カウンターが配置されるようにコマンド バッファーを (必要に応じて) パディングします。
	// バッファ内の有効な場所にあります。
	static inline UINT AlignForUavCounter(UINT bufferSize)
	{
		const UINT alignment = D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT;
		return (bufferSize + (alignment - 1)) & ~(alignment - 1);
	}
};

