#pragma once
#include "KDirectXCommon.h"
#include "Vector3.h"
#include "Vector2.h"

/**
 * @file KVertex.h
 * @brief 頂点
 * @author 飯島 薫
 */

// 頂点データ構造体
struct VertexPosNormalUV 
{
	KMyMath::Vector3 pos;	 // xyz座標
	KMyMath::Vector3 normal; // 法線ベクトル
	KMyMath::Vector2 uv;	 // uv座標
};

namespace MesiEngine {
class KVertex {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="dev"></param>
	/// <param name="vertices"></param>
	/// <param name="indices"></param>
	KVertex(
	    ID3D12Device* dev, std::vector<VertexPosNormalUV>& vertices,
	    std::vector<unsigned short>& indices);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dev"></param>
	/// <param name="vertices"></param>
	/// <param name="indices"></param>
	void KVertexInit(
	    ID3D12Device* dev, std::vector<VertexPosNormalUV>& vertices,
	    std::vector<unsigned short>& indices);

	// ゲッター
	const ID3D12Resource* GetVertBuff() const;
	const ID3D12Resource* GetIndexBuff() const;
	const D3D12_VERTEX_BUFFER_VIEW& GetVertBuffView() const;
	const D3D12_INDEX_BUFFER_VIEW& GetIndexBuffView() const;

private:
	// 頂点バッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;

	// インデックスバッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};

	HREFTYPE result;
};
}

