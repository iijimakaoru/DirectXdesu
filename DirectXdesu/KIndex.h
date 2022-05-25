#pragma once
#include "KVertex.h"

// インデックスデータ
static unsigned short indices[] = {
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

class KIndex
{
public:
	KIndex(KDirectInit dx, KVertex vertex);
	void IndexInit(KDirectInit dx, KVertex vertex);

	// インデックスデータ全体のサイズ
	UINT sizeIB;
	// インデックスバッファの生成
	ID3D12Resource* indexBuff = nullptr;
	// インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
};

