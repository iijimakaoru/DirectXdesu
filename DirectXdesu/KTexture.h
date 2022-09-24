#pragma once
#include "KVertex.h"
#include <string>

class KTexture
{
public:
	KTexture(ID3D12Device* dev, const wchar_t* msg,const wchar_t* msg2);
	void LoadTexture(const wchar_t* msg, const wchar_t* msg2);
	void GeneMipMap();
	void SetTextureBuff();
	void GeneTextureBuff(ID3D12Device* dev);
	void SendData();
	void SetDescHeap();
	void GeneDescHeap(ID3D12Device* dev);
	void GetSrvHandle(ID3D12Device* dev);
	void SetSRV();
	void CreateSRV(ID3D12Device* dev);

	TexMetadata metadata{};
	ScratchImage scraychImg{};
	TexMetadata metadata2{};
	ScratchImage scraychImg2{};

	ScratchImage mipChain{};
	ScratchImage mipChain2{};

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	D3D12_RESOURCE_DESC textureResourceDesc2{};
	// テクスチャバッファの生成
	ID3D12Resource* texBuff = nullptr;
	ID3D12Resource* texBuff2 = nullptr;
	// SRVの最大個数
	const size_t kMaxSRVCount = 2056;
	// デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	// 設定を元にSRV用デスクリプタヒープを生成
	ID3D12DescriptorHeap* srvHeap = nullptr;
	// SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle2;
	// シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};

	UINT incrementSize;

	HRESULT result;
};

