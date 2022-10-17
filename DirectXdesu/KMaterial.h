#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <wrl.h>

using namespace Microsoft::WRL;

using namespace DirectX;

// 定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial {
	XMFLOAT4 color; // 色
};

class KMaterial
{
public:
	KMaterial();
	KMaterial(ID3D12Device* dev);
	void Initialize(ID3D12Device* dev);
	void Update();

	// 画像の色
	float colorR = 1.0f;
	float colorG = 1.0f;
	float colorB = 1.0f;
	float colorA = 1.0f;
	// ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	// 定数バッファの生成
	ComPtr<ID3D12Resource> constBufferMaterial;
	// 定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	HRESULT result;
};

