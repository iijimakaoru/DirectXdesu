#pragma once
#include "TextureManager.h"
#include <array>

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

private:
};
