#pragma once
#include "KVertex.h"
#include <DirectXTex.h>
#include <string>
#include <wrl.h>

using namespace Microsoft::WRL;

using namespace DirectX;

using namespace std;

class KTexture
{
public:
	KTexture() {};
	void CreateTexture(const std::string& directoryPath, const std::string& filename);
	void LoadTexture(const std::string& directoryPath, const std::string& filename);
	void GeneMipMap();
	void SetTextureBuff();
	void GeneTextureBuff();
	void SendData();
	void SetDescHeap();
	void GeneDescHeap();
	void GetSrvHandle();
	void SetSRV();
	void CreateSRV();

	TexMetadata metadata{};
	ScratchImage scraychImg{};

	ScratchImage mipChain{};

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	// テクスチャバッファの生成
	ComPtr<ID3D12Resource> texBuff;
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

