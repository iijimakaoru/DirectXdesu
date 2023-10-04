#pragma once
#include "KDirectXCommon.h"
#include "Vector3.h"
#include "Vector2.h"

using namespace Microsoft::WRL;

// 頂点データ構造体
struct VertexPosNormalUV 
{
	KMyMath::Vector3 pos;	 // xyz座標
	KMyMath::Vector3 normal; // 法線ベクトル
	KMyMath::Vector2 uv;	 // uv座標
};

class KVertex
{
public:
	KVertex() {}
	KVertex(ID3D12Device* dev, std::vector<VertexPosNormalUV>& vertices, std::vector<unsigned short>& indices);
	void KVertexInit(ID3D12Device* dev, std::vector<VertexPosNormalUV>& vertices, std::vector<unsigned short>& indices);
	
	// 頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff;
	
	// インデックスバッファの生成
	ComPtr<ID3D12Resource> indexBuff;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	HREFTYPE result;
};

