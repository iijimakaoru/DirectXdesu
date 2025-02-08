#pragma once
#include "TextureManager.h"
#include <array>
#include <unordered_map>
#include <memory>
#include "KModel.h"

class ModelManager {
public:
	static ModelManager* GetInstance();

private:
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager&) = delete;
	const ModelManager& operator=(const ModelManager&) = delete;

public:
	/****************************************************************************
		@brief			objモデルをロード
		@param [in]		modelname　モデルのファイル名(拡張子なし)
		@param [in]		pName　モデルの名前(任意)
		@return			成功:モデルのポインタ、失敗:nullptr
	/****************************************************************************/
	static KModel* Load(const std::string& modelname, const char* pName = nullptr, bool smoothing = false);

	/****************************************************************************
		@brief			objモデルを取得
		@param [in]		mapName　モデルのファイル名(拡張子なし)orモデルの名前
		@return			成功:モデルのポインタ、失敗:nullptr
	/****************************************************************************/
	KModel* GetModels(const std::string& mapName);

private:

	KModel* _Load(const std::string& modelname, const char* pName,bool smoothing);

	KModel* _Find(const std::string& modelname, const char* pName);

private:

	std::unordered_map<size_t, size_t>						modelNames_;	//!< モデルに名前があった場合に使う(key:名前のハッシュ値 value:ファイルパスのハッシュ値)
	std::unordered_map<size_t, std::unique_ptr<KModel>>		models_;		//!< モデル配列(key:ファイルパスのハッシュ値 value:ファイルパスのハッシュ値)
};
