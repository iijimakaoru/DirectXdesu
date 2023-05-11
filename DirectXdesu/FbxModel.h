#pragma once
#include <string>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <vector>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

#include "KVertex.h"
#include "KTexture.h"

// ノード
struct Node
{
	// 名前
	std::string name;
	// ローカルスケール
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	// ローカル回転角
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	// ローカル移動
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	// ローカル変形行列
	DirectX::XMMATRIX transform;
	// グローバル変形行列
	DirectX::XMMATRIX globalTransform;
	// 親ノード
	Node* parent = nullptr;
};

class FbxModel
{
public:
	// フレンドクラス
	friend class FbxLoader;

private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	// std::省略
	using string = std::string;
	template <class T> using vector = std::vector<T>;

private:
	// モデル名
	std::string name;
	// ノード配列
	std::vector<Node> nodes;
	// メッシュを持つノード
	Node* meshNode = nullptr;
	// 頂点データ配列
	std::vector<VertexPosNormalUV> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short> indices;
	// アンビエント係数
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	// ディフーズ係数
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };

	std::unique_ptr<KVertex> vertexs = make_unique<KVertex>();

	KTexture texture;

public: // 関数
	// メンバ
	void CreateModel();
	void Draw();

	// ゲッター
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }
};

