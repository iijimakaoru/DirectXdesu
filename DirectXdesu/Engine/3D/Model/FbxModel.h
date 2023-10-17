#pragma once
#include "KVertex.h"
#include "TextureManager.h"

#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4365)
#pragma warning(disable: 4266)
#pragma warning(disable: 4514)
#pragma warning(disable: 4820)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 5027)
#pragma warning(disable: 5026)
#pragma warning(disable: 5267)
#pragma warning(disable: 4619)
#include <fbxsdk.h>
#pragma warning(pop)

/**
 * @file FbxModel.h
 * @brief Fbxのモデル
 * @author 飯島 薫
 */

// ノード
struct Node
{
	// 名前
	std::string name;
	// ローカルスケール
	KMyMath::Vector4 scaling = { 1,1,1,0 };
	// ローカル回転角
	KMyMath::Vector4 rotation = { 0,0,0,0 };
	// ローカル移動
	KMyMath::Vector4 translation = { 0,0,0,1 };
	// ローカル変形行列
	KMyMath::Matrix4 transform;
	// グローバル変形行列
	KMyMath::Matrix4 globalTransform;
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
		KMyMath::Matrix4 invInitialPose;
		// クラスター
		FbxCluster* fbxCluster;
		// コントラクタ
		Bone(const std::string& name_);
	};

	struct VertexPosNormalUVSkin
	{
		KMyMath::Vector3 pos;	 // xyz座標
		KMyMath::Vector3 normal; // 法線ベクトル
		KMyMath::Vector2 uv;	 // uv座標
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

	std::unique_ptr<KVertex> vertexs;

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

	std::vector<Bone> bones;

	// FBXシーン
	FbxScene* fbxScene = nullptr;

public: // 関数
	// メンバ

	// バッファ生成
	void CreateBuffer();

	// テクスチャ読み込み
	void LoadTexture();

	// 描画
	void Draw();

	FbxModel();
	~FbxModel();

	// FbxSceneゲッター
	FbxScene* GetFbxScene();

	// ゲッター
	const KMyMath::Matrix4& GetModelTransform();
	std::vector<Bone>& GetBones();
};

