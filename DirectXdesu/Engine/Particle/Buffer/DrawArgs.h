#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include "Emitter.h"

class DrawArgs
{
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> RWDrawArgs = nullptr;

	CD3DX12_CPU_DESCRIPTOR_HANDLE DrawArgsCPUUAV;
	CD3DX12_GPU_DESCRIPTOR_HANDLE DrawArgsGPUUAV;

	D3D12_RESOURCE_STATES resourseState = D3D12_RESOURCE_STATE_COMMON;

public:
	void Create(ID3D12DescriptorHeap* uavHeap);
	ID3D12Resource* GetDrawArgs();
	CD3DX12_CPU_DESCRIPTOR_HANDLE GetCPUUAV();
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetGPUUAV();
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

