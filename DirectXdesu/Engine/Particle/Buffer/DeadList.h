#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include "Emitter.h"

class DeadList
{
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> ACDeadList = nullptr;

	CD3DX12_CPU_DESCRIPTOR_HANDLE ACDeadListCPUUAV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE ACDeadListGPUUAV;

public:
	void Create(ID3D12DescriptorHeap* uavHeap, uint32_t particleMax);
	ID3D12Resource* GetDeadList();
	CD3DX12_CPU_DESCRIPTOR_HANDLE GetCPUUAV();
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetGPUUAV();

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

