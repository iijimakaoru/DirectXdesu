#pragma once
#include <DirectXMath.h>
#include "KDirectXCommon.h"
#include "Vector3.h"
#include "Vector2.h"
#include <vector>
#include <wrl.h>

using namespace Microsoft::WRL;

using namespace DirectX;

//std::vector<int>;

// 頂点データ構造体
struct VertexPosNormalUV {
	XMFLOAT3 pos;	 // xyz座標
	XMFLOAT3 normal; // 法線ベクトル
	XMFLOAT2 uv;	 // uv座標
};

class KVertex
{
public:
	KVertex() {}
	KVertex(ID3D12Device* dev, std::vector<VertexPosNormalUV>& vertices, std::vector<unsigned short>& indices);
	void KVertexInit(ID3D12Device* dev, std::vector<VertexPosNormalUV>& vertices, std::vector<unsigned short>& indices);

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB;
	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	// 頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff;

	VertexPosNormalUV* vertMap = nullptr;
	// インデックスデータ全体のサイズ
	UINT sizeIB;
	// インデックスバッファの生成
	ComPtr<ID3D12Resource> indexBuff;
	// インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	HREFTYPE result;
};

