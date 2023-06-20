#include "KDepthStencilBuffer.h"
#include <d3dx12.h>

bool KDepthStencilBuffer::Create(UINT w, UINT h, DXGI_FORMAT format)
{
	height = h;
	width = w;

	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = format;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	CD3DX12_RESOURCE_DESC desc(
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0,
		w, h,
		1, 1,
		format,
		1, 0,
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

	CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	//深度バッファ作成
	HRESULT hr = device->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(texture.GetAddressOf())
	);


	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = format;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	//DSV制作
	handle.ptr = dsvHeap->CreateDSV(dsvDesc, texture.Get());


	if (FAILED(hr))
	{
		MessageBoxA(nullptr, "深度ステンシルバッファの作成に失敗", "エラー", MB_OK);
		return false;
	}

	return true;
}

KDepthStencilBuffer::KDepthStencilBuffer(ID3D12Device* d3dDevice, KDsvDescriptorHeap* descriptorHeap) : device(d3dDevice), dsvHeap(descriptorHeap)
{
}

ID3D12Resource* KDepthStencilBuffer::GetTexture() const
{
    return texture.Get();
}

const D3D12_CPU_DESCRIPTOR_HANDLE& KDepthStencilBuffer::GetHandle()
{
	return handle;
}
