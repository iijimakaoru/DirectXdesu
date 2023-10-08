#include "KRtvDescriptorHeap.h"
#include "KDirectXCommon.h"

void KRtvDescriptorHeap::Initialize()
{
	device = KDirectXCommon::GetInstance()->GetDev();
	HRESULT result = 0;

	maxRTV = 2048;
	countRTV = 0;

	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = static_cast<UINT>(maxRTV);

	// 設定を元にSRV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(descriptorHeap.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//SRVヒープの先頭ハンドルを取得
	startCpuHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
}

UINT64 KRtvDescriptorHeap::CreateRTV(D3D12_RENDER_TARGET_VIEW_DESC& desc, ID3D12Resource* resource)
{
	if (countRTV > maxRTV)
	{
		assert(0);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE	handle{};

	handle.ptr = startCpuHandle.ptr + (static_cast<UINT64>(countRTV) * incrementSize);

	device->CreateRenderTargetView(resource, &desc, handle);
	countRTV++;

	return handle.ptr;
}

ID3D12DescriptorHeap* KRtvDescriptorHeap::GetHeap()
{
	return descriptorHeap.Get();
}

UINT KRtvDescriptorHeap::GetIncrementSize()
{
	return incrementSize;
}
