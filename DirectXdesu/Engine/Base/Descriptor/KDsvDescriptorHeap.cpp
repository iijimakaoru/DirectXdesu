#include "KDsvDescriptorHeap.h"
#include "KDirectXCommon.h"

void KDsvDescriptorHeap::Initialize()
{
	device = KDirectXCommon::GetInstance()->GetDev();
	HRESULT result = 0;

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.NumDescriptors = static_cast<UINT>(maxDSV);

	// 設定を元にSRV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(descriptorHeap.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	//SRVヒープの先頭ハンドルを取得
	startCpuHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
}

UINT64 KDsvDescriptorHeap::CreateDSV(D3D12_DEPTH_STENCIL_VIEW_DESC& desc, ID3D12Resource* resource)
{
	if (countDSV > maxDSV)
	{
		assert(0);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE	handle{};

	handle.ptr = startCpuHandle.ptr + (static_cast<UINT64>(countDSV) * incrementSize);

	device->CreateDepthStencilView(resource, &desc, handle);
	countDSV++;

	return handle.ptr;
}

ID3D12DescriptorHeap* KDsvDescriptorHeap::GetHeap()
{
	return descriptorHeap.Get();
}

UINT KDsvDescriptorHeap::GetIncrementSize()
{
	return incrementSize;
}
