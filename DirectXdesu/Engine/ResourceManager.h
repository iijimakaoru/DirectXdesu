#pragma once
#include "TextureManager.h"
#include <array>
#include <map>
#include <memory>
#include "KModel.h"

class ResourceManager {
public:
	static ResourceManager* GetInstance();

private:
	ResourceManager() = default;
	~ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	const ResourceManager& operator=(const ResourceManager&) = delete;

public:
	void Init();

	KModel* GetModels(std::string mapName);

private:
	std::map<std::string, std::unique_ptr<KModel>> models_;
};
