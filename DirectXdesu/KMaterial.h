#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXTex.h>

using namespace DirectX;

// 定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial {
	XMFLOAT4 color; // 色
};

class KMaterial
{
public:
	KMaterial(HRESULT result, ID3D12Device* dev);
	void Initialize(HRESULT result, ID3D12Device* dev);
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
	ID3D12Resource* constBufferMaterial = nullptr;
	// 定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;
};

