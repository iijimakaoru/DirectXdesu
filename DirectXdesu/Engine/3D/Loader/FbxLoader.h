#pragma once
#include "fbxsdk.h"
#include "FbxModel.h"

class FbxLoader
{
private:// エイリアス
	// stdを省略
	using string = std::string;

	/// <summary>
	/// メンバ関数
	/// </summary>
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

	/// <summary>
	/// Fbxモデル読み込み
	/// </summary>
	FbxModel* LoadModelFromFile(const string& modelName);

	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name = "model">読み込み先モデルオブジェクト</param>
	/// <param name = "fbxNode">解析対象のノード</param>
	void ParseNodeRecursive(FbxModel* model, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name = "model">読み込み先モデルオブジェクト</param>
	/// <param name = "fbxNode">解析対象のノード</param>
	void ParseMesh(FbxModel* model, FbxNode* fbxNode);

	// 頂点座標読み取り
	void ParseMeshVertices(FbxModel* model, FbxMesh* fbxMesh);

	// 面情報読み取り
	void ParseMeshFaces(FbxModel* model, FbxMesh* fbxMesh);

	// マテリアル読み込み
	void ParseMaterial(FbxModel* model, FbxNode* fbxNode);

	// テクスチャ読み込み
	void LoadTexture(FbxModel* model, const std::string& fullpath);

	// ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path);

	// スキニング情報読み取り
	void ParseSkin(FbxModel* model_, FbxMesh* fbxMesh);

/// <summary>
/// FBXの行列をXMMatrixに変換
/// </summary>
	static void ConvertMatrixFromFbx(KMyMath::Matrix4* dst, const FbxAMatrix& src);

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

	/// <summary>
	/// メンバ変数
	/// </summary>
private:
	// Fbxマネージャー
	FbxManager* fbxManager = nullptr;
	// Fbxインポーター
	FbxImporter* fbxImporter = nullptr;

public:// 定数
	// モデル格納ルートパス
	static const string baseDirectory;
	// テクスチャがない場合のテクスチャファイル名
	static const string defaultTextureFileName;
};