#pragma once

#include "fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>

#include <string>

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
	void LoadModelFromFile(const string& modelName);

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
};