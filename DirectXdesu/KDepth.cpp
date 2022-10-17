#include "KDepth.h"

KDepth::KDepth(ID3D12Device* dev, int window_width, int window_height) {
	SetResource(window_width, window_height);
	HeapDepth();
	SetClwarDepth();
	GeneResource(dev);
	CreatDevHeap(dev);
	CreatDepthView(dev);
}

void KDepth::SetResource(int window_width, int window_height) {
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = window_width; // レンダーターゲットに合わせる
	depthResourceDesc.Height = window_height; // レンダーターゲットに合わせる
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // デプスステンシル
}

void KDepth::HeapDepth() {
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
}

void KDepth::SetClwarDepth() {
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
}

void KDepth::GeneResource(ID3D12Device* dev) {
	depthBuff = nullptr;
	result = dev->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));
}

void KDepth::CreatDevHeap(ID3D12Device* dev) {
	dsvHeapDesc.NumDescriptors = 1; // 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // デプスステンシルビュー
	dsvHeap = nullptr;
	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
}

void KDepth::CreatDepthView(ID3D12Device* dev) {
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());
}