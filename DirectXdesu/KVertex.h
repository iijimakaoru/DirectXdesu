#pragma once
#include <DirectXMath.h>
#include "KDirectInit.h"
#include "Vector3.h"

using namespace DirectX;

// 頂点データ構造体
struct Vertex {
	XMFLOAT3 pos;	 // xyz座標
	XMFLOAT3 normal; // 法線ベクトル
	XMFLOAT2 uv;	 // uv座標
};

// 頂点データ
static XMFLOAT3 vertices[] = {
	{-0.5f,-0.5f,0.0f},
	{-0.5f,+0.5f,0.0f},
	{+0.5f,-0.5f,0.0f}
};

// インデックスデータ
static unsigned short indices[] = {
	// 前
	 0, 1, 2, // 三角形1つ目
};

class KVertex
{
public:
	KVertex(ID3D12Device* dev);
	void KVertexInit(ID3D12Device* dev);
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

	HRESULT result;
};

