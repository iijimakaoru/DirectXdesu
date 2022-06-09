#pragma once
#include "KDirectInit.h"
#include "KVertex.h"

class KTexture
{
public:
	KTexture(KDirectInit dx, KVertex vertex);
	void LoadTexture(KDirectInit dx,int i);
	void GeneMipMap(KDirectInit dx);
	void SetTextureBuff();
	void GeneTextureBuff(KDirectInit dx, int i);
	void SendData(KDirectInit dx, int i);
	void SetDescHeap();
	void GeneDescHeap(KDirectInit dx);
	void GetSrvHandle(KDirectInit dx);
	void SetSRV(KDirectInit dx,KVertex vertex);
	void CreateSRV(KDirectInit dx, int i);

	TexMetadata metadata{};
	ScratchImage scraychImg{};

	ScratchImage mipChain{};

	const wchar_t* msg[2] = { L"Resources/kitanai.jpg",L"Resources/haikei.jpg" };

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	// テクスチャバッファの生成
	ID3D12Resource* texBuff[2] = { nullptr };
	// SRVの最大個数
	const size_t kMaxSRVCount = 2056;
	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	// 設定を元にSRV用デスクリプタヒープを生成
	ID3D12DescriptorHeap* srvHeap = nullptr;
	// SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle[2];
	// シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
};

