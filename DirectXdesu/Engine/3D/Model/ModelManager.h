#pragma once
#include "TextureManager.h"
#include <array>
#include <map>
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
	void Init();

	KModel* GetModels(std::string mapName);

private:
	std::map<std::string, std::unique_ptr<KModel>> models_;
};
