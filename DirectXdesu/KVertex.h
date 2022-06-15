#pragma once
#include <DirectXMath.h>
#include "KDirectInit.h"
#include "Vector3.h"
#include <vector>
//#include "KModel.h"

using namespace DirectX;

//std::vector<int>;

// 頂点データ構造体
struct Vertex {
	XMFLOAT3 pos;	 // xyz座標
	XMFLOAT3 normal; // 法線ベクトル
	XMFLOAT2 uv;	 // uv座標
};
// 頂点データ
static std::vector<Vertex> vertices = {
	//  x	  y	    z	 n    u	   v
	// 前
	{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // 左下
	{{-5.0f, 5.0f,-5.0f},{},{0.0f,0.0f}}, // 左上
	{{ 5.0f,-5.0f,-5.0f},{},{1.0f,1.0f}}, // 右下
	{{ 5.0f, 5.0f,-5.0f},{},{1.0f,0.0f}}, // 右上
	// 後
	{{ 5.0f,-5.0f, 5.0f},{},{1.0f,1.0f}}, // 右下
	{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}, // 右上
	{{-5.0f,-5.0f, 5.0f},{},{0.0f,1.0f}}, // 左下
	{{-5.0f, 5.0f, 5.0f},{},{0.0f,0.0f}}, // 左上
	// 左
	{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // 左下
	{{-5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}}, // 左上
	{{-5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}}, // 右下
	{{-5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}, // 右上
	// 右
	{{ 5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}}, // 右下
	{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}, // 右上
	{{ 5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // 左下
	{{ 5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}}, // 左上
	// 下
	{{ 5.0f,-5.0f,-5.0f},{},{1.0f,1.0f}}, // 右下
	{{ 5.0f,-5.0f, 5.0f},{},{1.0f,0.0f}}, // 右上
	{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // 左下
	{{-5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}}, // 左上
	// 上
	{{-5.0f, 5.0f,-5.0f},{},{0.0f,1.0f}}, // 左下
	{{-5.0f, 5.0f, 5.0f},{},{0.0f,0.0f}}, // 左上
	{{ 5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}}, // 右下
	{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}  // 右上
};

// インデックスデータ
static std::vector<short> indices = {
	// 前
	 0, 1, 2, // 三角形1つ目
	 2, 1, 3, // 三角形2つ目
	// 後
	 4, 5, 6,
	 6, 5, 7,
	 // 左
	  8, 9,10,
	 10, 9,11,
	 // 右
	 12,13,14,
	 14,13,15,
	 // 下
	 16,17,18,
	 18,17,19,
	 // 上
	 20,21,22,
	 22,21,23
};

class KVertex
{
public:
	KVertex(KDirectInit dx);
	void KVertexInit(KDirectInit dx);
	void VertMap();
	void CreateVBView();

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB;
	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	// 頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	// インデックスデータ全体のサイズ
	UINT sizeIB;
	// インデックスバッファの生成
	ID3D12Resource* indexBuff = nullptr;
	// インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[3] = {
		{// xyz座標
			"POSITION",										// セマンティック名
			0,												// 同じセマンティック名が複数あるときに使うインデックス
			DXGI_FORMAT_R32G32B32_FLOAT,					// 要素数とビット数を表す
			0,												// 入力スロットインデックス
			D3D12_APPEND_ALIGNED_ELEMENT,					// データのオフセット
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		// 入力データ種別
			0												// 一度に描画するインスタンス数
		},
		{// 法線ベクトル
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{// uv座標
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};
};

