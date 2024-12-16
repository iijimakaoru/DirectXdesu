#pragma once
#include <d3d12.h>
#include <stdint.h>

class SRVDescriptorHeap
{
private:
	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	char PADING[4]{};

	uint32_t maxSRV = 0;
	uint32_t maxUAV = 0;
	uint32_t maxCBV = 0;

	uint32_t countSRV = 0;
	uint32_t countUAV = 0;
	uint32_t countCBV = 0;

	uint32_t incrementSize = 0;

	HRESULT result = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE startCpuHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE startGpuHandle{};

public:
	

	SRVDescriptorHeap() = default;
	~SRVDescriptorHeap() = default;
};

