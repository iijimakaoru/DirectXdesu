#include "ModelManager.h"

ModelManager* ModelManager::GetInstance() {
	static ModelManager instance;

	return &instance;
}

KModel* ModelManager::Load(const std::string& modelname, const char* pName, bool smoothing)
{
	return GetInstance()->_Load(modelname, pName, smoothing);
}

KModel* ModelManager::GetModels(const std::string& mapName)
{
	KModel* result = _Find(mapName, mapName.c_str());

	if (result != nullptr)
	{
		return result;
	}

	return nullptr;

}

KModel* ModelManager::_Load(const std::string& modelname, const char* pName, bool smoothing)
{
	KModel* result = _Find(modelname, pName);

	if (result != nullptr)
	{
		return result;
	}

	size_t pathHash = std::hash<std::string>()("Resources/obj/" + modelname);

	const auto& resultPair = models_.insert(std::pair(pathHash, std::make_unique<MtlObj>(modelname, smoothing)));
	auto& resultModel = resultPair.first->second;
	result = resultModel.get();

	if (pName != nullptr)
	{
		size_t pNameHash = std::hash<std::string_view>()(pName);

		modelNames_.insert(std::pair(pNameHash, pathHash));
	}

	return result;
}

KModel* ModelManager::_Find(const std::string& modelname, const char* pName)
{
	size_t pNameHash = std::hash<std::string_view>()(pName);

	if (modelNames_.find(pNameHash) != modelNames_.end())
	{
		size_t pathHash = modelNames_[pNameHash];
		return models_[pathHash].get();
	}

	size_t pathHash = std::hash<std::string>()("Resources/obj/" + modelname);

	if (models_.find(pathHash) != models_.end())
	{
		return models_[pathHash].get();
	}

	return nullptr;
}
