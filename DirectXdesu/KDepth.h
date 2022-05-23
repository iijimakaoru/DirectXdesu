#pragma once
#include <d3d12.h>
#include "KDirectInit.h"
class KDepth
{
public:
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

	KDepth(KDirectInit dx, KWindow win);
	void SetResource(KWindow win);
	void HeapDepth();
	void SetClwarDepth();
	void GeneResource(KDirectInit dx);
	void CreatDevHeap(KDirectInit dx);
	void CreatDepthView(KDirectInit dx);
};