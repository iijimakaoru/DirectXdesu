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

#include <fbxsdk.h>

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

	static const int MAX_BONE_INDICES = 4;

	struct Bone
	{
		std::string name;
		// 初期姿勢の逆行列
		DirectX::XMMATRIX invInitialPose;
		// クラスター
		FbxCluster* fbxCluster;
		// コントラクタ
		Bone(const std::string& name_)
		{
			name = name_;
		}
	};

	struct VertexPosNormalUVSkin
	{
		XMFLOAT3 pos;	 // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;	 // uv座標
		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];
	};

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
	std::vector<VertexPosNormalUVSkin> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short> indices;
	// アンビエント係数
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	// ディフーズ係数
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };

	std::unique_ptr<KVertex> vertexs = make_unique<KVertex>();

	ScratchImage scratchImg;

	TexMetadata metadata;

	// 
	ComPtr<ID3D12Resource> vertBuff;

	ComPtr<ID3D12Resource> indexBuff;

	// テクスチャバッファの生成
	ComPtr<ID3D12Resource> texBuff;

	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// 設定を元にSRV用デスクリプタヒープを生成
	ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;

	//KTexture texture;

	std::vector<Bone> bones;

	// FBXシーン
	FbxScene* fbxScene = nullptr;

public: // 関数
	// メンバ
	void CreateBuffer();
	void LoadTexture();
	void Draw();

	~FbxModel();

	FbxScene* GetFbxScene() { return fbxScene; }

	// ゲッター
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }
	std::vector<Bone>& GetBones() { return bones; }
};

