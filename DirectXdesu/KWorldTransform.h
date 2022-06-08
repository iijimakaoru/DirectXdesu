#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include "Vector3.h"

using namespace DirectX;

// 定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform {
	XMMATRIX mat; // 3D変換行列
};

// 3Dオブジェクトの数
const size_t kObjectCount = 1;

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
	KWorldTransform(HRESULT result, ID3D12Device* dev);
	void Initialize(HRESULT result, ID3D12Device* dev);
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);
	void Draw(ID3D12GraphicsCommandList* cmdList, D3D12_VERTEX_BUFFER_VIEW& vbview,
		D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices);

	Vector3 rotResult[kObjectCount];
	// ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	// 3Dオブジェクトの配列
	Object3d object3d[kObjectCount];
};

