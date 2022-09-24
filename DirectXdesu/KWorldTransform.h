#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include "Vector3.h"
#include "KModel.h"
#include "KTexture.h"
#include "KMaterial.h"
//#include <memory>

using namespace DirectX;

// 定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform {
	XMMATRIX mat; // 3D変換行列
};

struct Object3d {
	// 定数バッファ(行列)
	ID3D12Resource* constBuffTransform = {};
	// 定数バッファマップ(行列)
	ConstBufferDataTransform* constMapTransform = {};
	// アフィン変換
	Vector3 scale = { 1,1,1 };
	Vector3 rot = { 0,0,0 };
	Vector3 pos = { 0,0,0 };
	// ワールド変換行列
	XMMATRIX matWorld = {};
	// 親オブジェクトへのポインタ
	Object3d* parent = nullptr;
};

class KWorldTransform
{
public:
	KWorldTransform();
	KWorldTransform(ID3D12Device& dev);
	void SetTexture(KTexture* texture);
	void SetModel(KModel* model);
	void Initialize(ID3D12Device& dev);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void SecoundDraw(ID3D12GraphicsCommandList* cmdList);

	Vector3 rotResult;
	// ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	// 3Dオブジェクトの配列
	Object3d transform;

	HRESULT result;

	KModel* model = nullptr;

	KTexture* texture = nullptr;

	// マテリアル
	KMaterial* material = nullptr;
};

