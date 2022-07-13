#pragma once
#include "KVertex.h"

class KTexture
{
public:
	KTexture(ID3D12Device* dev,KVertex vertex);
	void LoadTexture();
	void GeneMipMap();
	void SetTextureBuff();
	void GeneTextureBuff(ID3D12Device* dev);
	void SendData();
	void SetDescHeap();
	void GeneDescHeap(ID3D12Device* dev);
	void GetSrvHandle();
	void SetSRV(KVertex vertex);
	void CreateSRV(ID3D12Device* dev);

	TexMetadata metadata{};
	ScratchImage scraychImg{};

	ScratchImage mipChain{};

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	// テクスチャバッファの生成
	ID3D12Resource* texBuff = nullptr;
	// SRVの最大個数
	const size_t kMaxSRVCount = 2056;
	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	// 設定を元にSRV用デスクリプタヒープを生成
	ID3D12DescriptorHeap* srvHeap = nullptr;
	// SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
	// シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	HRESULT result;
};

