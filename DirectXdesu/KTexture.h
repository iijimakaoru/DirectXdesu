#pragma once
#include "KDirectInit.h"
#include "KVertex.h"

class KTexture
{
public:
	KTexture(KDirectInit dx, KVertex vertex);
	void LoadTexture(KDirectInit dx);
	void GeneMipMap(KDirectInit dx);
	void SetTextureBuff();
	void GeneTextureBuff(KDirectInit dx);
	void SendData(KDirectInit dx);
	void SetDescHeap();
	void GeneDescHeap(KDirectInit dx);
	void GetSrvHandle();
	void SetSRV(KDirectInit dx,KVertex vertex);
	void CreateSRV(KDirectInit dx);

	TexMetadata metadata{};
	ScratchImage scraychImg{};

	TexMetadata metadata2{};
	ScratchImage scraychImg2{};

	ScratchImage mipChain{};

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	D3D12_RESOURCE_DESC textureResourceDesc2{};
	// テクスチャバッファの生成
	ID3D12Resource* texBuff = { nullptr };
	ID3D12Resource* texBuff2 = { nullptr };
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
};

