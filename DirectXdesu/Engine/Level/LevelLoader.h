#pragma once
#pragma warning(push)
#pragma warning(disable: 4820)
#pragma warning(disable: 4626)
#pragma warning(disable: 5027)
#pragma warning(disable: 4623)
#pragma warning(disable: 4668)
#pragma warning(disable: 4625)
#pragma warning(disable: 4514)
#include "json.hpp"
#include <DirectXMath.h>
#pragma warning(pop)

// レベルデータ
struct LevelData 
{
	struct ObjectData 
	{
		// ファイル名
		std::string fileName;
		// 平行移動
		DirectX::XMVECTOR translation;
		// 回転角
		DirectX::XMVECTOR rotation;
		// スケーリング
		DirectX::XMVECTOR scaling;
	};

	// オブジェクト配列
	std::vector<ObjectData> objects;
};

// レベルデータのローダー
class LevelLoader 
{
public:// 定数
	// デフォルトの読み込みディレクトリ
	static const std::string kDefaultBaseDirectory;
	// ファイル拡張子
	static const std::string kExtension;

public:// メンバ関数

	/// <summary>
	/// レベルデータファイルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	static LevelData* LoadFile(const std::string& fileName);
};

