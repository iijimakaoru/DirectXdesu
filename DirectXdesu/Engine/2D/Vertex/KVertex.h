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
struct Vertex {
	KMyMath::Vector3 position;
	KMyMath::Vector3 normal;
};

struct VertexPosNormalUV 
{
	KMyMath::Vector3 pos;	 // xyz座標
	KMyMath::Vector3 normal; // 法線ベクトル
	KMyMath::Vector2 uv;	 // uv座標
};

class KVertex
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="dev"></param>
	/// <param name="vertices"></param>
	/// <param name="indices"></param>
	KVertex(ID3D12Device* dev, 
		std::vector<VertexPosNormalUV>& vertices, 
		std::vector<unsigned short>& indices);
	KVertex(ID3D12Device* dev,
		std::vector<Vertex>& vertices);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dev"></param>
	/// <param name="vertices"></param>
	/// <param name="indices"></param>
	void KVertexInit(ID3D12Device* dev,
		std::vector<VertexPosNormalUV>& vertices,
		std::vector<unsigned short>& indices);
	void KVertexInit(ID3D12Device* dev,
		std::vector<Vertex>& vertices);

	// ゲッター
	const ID3D12Resource* GetVertBuff() const;
	const ID3D12Resource* GetIndexBuff() const;
	const D3D12_VERTEX_BUFFER_VIEW& GetVertBuffView() const;
	const D3D12_INDEX_BUFFER_VIEW& GetIndexBuffView() const;

	CD3DX12_GPU_DESCRIPTOR_HANDLE CreateDescripterSRV(ID3D12DescriptorHeap* descHeap);
	
private:
	// 頂点バッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;

	// インデックスバッファの生成
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;

	CD3DX12_GPU_DESCRIPTOR_HANDLE descripterSRV;

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};

	HREFTYPE result;

	uint32_t length;

	uint32_t singleSize;
};

