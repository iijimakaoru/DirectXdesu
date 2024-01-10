#include "KDescriptorHeap.h"
#include "KDirectXCommon.h"

using namespace MesiEngine;

void KDescriptorHeap::Initialize()
{
	device = KDirectXCommon::GetInstance()->GetDev();
	HRESULT result = 0;

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	srvHeapDesc.NumDescriptors = static_cast<UINT>(maxSRV + maxUAV + maxCBV);

	// 設定を元にSRV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(descriptorHeap.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//SRVヒープの先頭ハンドルを取得
	startCpuHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	startGpuHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
}

UINT64 KDescriptorHeap::CreateSRV(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, ID3D12Resource* resource)
{
	if (countSRV > maxSRV)
	{
		assert(0);
	}

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE	cpuHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE	gpuHandle{};

	//ハンドルのポインタずらし
	cpuHandle.ptr = startCpuHandle.ptr + (static_cast<UINT64>(countSRV) * incrementSize);
	gpuHandle.ptr = startGpuHandle.ptr + (static_cast<UINT64>(countSRV) * incrementSize);

	// ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateShaderResourceView(resource, &desc, cpuHandle);
	countSRV++;

	return gpuHandle.ptr;
}

KDescriptorHeap::DescriptorHeapViewHandle KDescriptorHeap::AddSRV()
{
	if (countSRV > maxSRV)
	{
		assert(0);
	}

	DescriptorHeapViewHandle handle{};

	//ハンドルのポインタずらし
	handle.cpuHandle.ptr = startCpuHandle.ptr + (static_cast<UINT64>(countSRV) * incrementSize);
	handle.gpuHandle.ptr = startGpuHandle.ptr + (static_cast<UINT64>(countSRV) * incrementSize);

	countSRV++;

	return handle;
}

UINT64 KDescriptorHeap::CreateUAV(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, ID3D12Resource* resource)
{
	if (countUAV > maxUAV)
	{
		assert(0);
	}

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE	cpuHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE	gpuHandle{};

	//ハンドルのポインタずらし
	cpuHandle.ptr = startCpuHandle.ptr + (static_cast<UINT64>(maxSRV + countUAV) * incrementSize);
	gpuHandle.ptr = startGpuHandle.ptr + (static_cast<UINT64>(maxSRV + countUAV) * incrementSize);

	// ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateUnorderedAccessView(resource, nullptr, &desc, cpuHandle);

	countUAV++;

	return gpuHandle.ptr;
}

KDescriptorHeap::DescriptorHeapViewHandle KDescriptorHeap::AddUAV()
{
	if (countUAV > maxUAV)
	{
		assert(0);
	}

	DescriptorHeapViewHandle handle{};

	//ハンドルのポインタずらし
	handle.cpuHandle.ptr = startCpuHandle.ptr + (static_cast<UINT64>(maxSRV + countUAV) * incrementSize);
	handle.gpuHandle.ptr = startGpuHandle.ptr + (static_cast<UINT64>(maxSRV + countUAV) * incrementSize);

	countUAV++;

	return handle;
}

UINT64 KDescriptorHeap::CreateCBV(D3D12_CONSTANT_BUFFER_VIEW_DESC& desc)
{
	if (countCBV > maxCBV)
	{
		assert(0);
	}

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE	cpuHandle{};
	D3D12_GPU_DESCRIPTOR_HANDLE	gpuHandle{};

	//ハンドルのポインタずらし
	cpuHandle.ptr = startCpuHandle.ptr + (static_cast<UINT64>(maxSRV + maxUAV + countCBV) * incrementSize);
	gpuHandle.ptr = startGpuHandle.ptr + (static_cast<UINT64>(maxSRV + maxUAV + countCBV) * incrementSize);

	// ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateConstantBufferView(&desc, cpuHandle);

	countCBV++;

	return gpuHandle.ptr;
}

KDescriptorHeap::DescriptorHeapViewHandle KDescriptorHeap::AddCBV()
{
	if (countCBV > maxCBV)
	{
		assert(0);
	}

	DescriptorHeapViewHandle handle{};

	//ハンドルのポインタずらし
	handle.cpuHandle.ptr = startCpuHandle.ptr + (static_cast<UINT64>(maxSRV + maxUAV + countCBV) * incrementSize);
	handle.gpuHandle.ptr = startGpuHandle.ptr + (static_cast<UINT64>(maxSRV + maxUAV + countCBV) * incrementSize);

	countCBV++;

	return handle;
}

ID3D12DescriptorHeap* KDescriptorHeap::GetHeap()
{
	return descriptorHeap.Get();
}
