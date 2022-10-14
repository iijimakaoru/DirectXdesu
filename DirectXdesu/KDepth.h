#pragma once
#include <d3d12.h>
#include "KDirectXCommon.h"
class KDepth
{
public:
	KDepth(ID3D12Device* devint, int window_width, int window_height);
	ID3D12DescriptorHeap* GetDevHeap() { return dsvHeap; }

private:
	HRESULT result;

	// リソース設定
	D3D12_RESOURCE_DESC depthResourceDesc{};

	// 深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};

	// 深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};

	// 深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

	ID3D12Resource* depthBuff{};

	ID3D12DescriptorHeap* dsvHeap{};

	void SetResource(int window_width, int window_height);
	void HeapDepth();
	void SetClwarDepth();
	void GeneResource(ID3D12Device* dev);
	void CreatDevHeap(ID3D12Device* dev);
	void CreatDepthView(ID3D12Device* dev);
};