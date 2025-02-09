#include "AnimationModelManager.h"
#include <PModelLoader.h>

using namespace MCBM;
AnimationModelManager* AnimationModelManager::GetInstance() {
	static AnimationModelManager instance;

	return &instance;
}

Skelton* AnimationModelManager::Load(const std::string& modelname, const char* pName)
{
	return GetInstance()->_Load(modelname, pName);
}

Skelton* AnimationModelManager::GetModel(const std::string& mapName)
{
	Skelton* result = _Find(mapName, mapName.c_str());

	if (result != nullptr)
	{
		return result;
	}

	return nullptr;

}

Skelton* AnimationModelManager::_Load(const std::string& modelname, const char* pName)
{
	Skelton* result = _Find(modelname, pName);

	if (result != nullptr)
	{
		return result;
	}

	size_t pathHash = std::hash<std::string>()("Resources/obj/" + modelname);


	std::unique_ptr<Skelton> tempModel = std::make_unique<Skelton>();
	PHONONLOADER::P_MODEL_DATA modelData;
	PHONONLOADER::PModelLoader::Load(&modelData, "obj/" + modelname);
	tempModel->SetDataFromLoader(modelData);
	const auto& resultPair = models_.insert(std::pair(pathHash, std::move(tempModel)));
	auto& resultModel = resultPair.first->second;
	result = resultModel.get();

	if (pName != nullptr)
	{
		size_t pNameHash = std::hash<std::string_view>()(pName);

		modelNames_.insert(std::pair(pNameHash, pathHash));
	}

	return result;
}

Skelton* AnimationModelManager::_Find(const std::string& modelname, const char* pName)
{
	if (pName)
	{
		size_t pNameHash = std::hash<std::string_view>()(pName);

		if (modelNames_.find(pNameHash) != modelNames_.end())
		{
			size_t pathHash = modelNames_[pNameHash];
			return models_[pathHash].get();
		}
	}
	size_t pathHash = std::hash<std::string>()("Resources/obj/" + modelname);

	if (models_.find(pathHash) != models_.end())
	{
		return models_[pathHash].get();
	}

	return nullptr;
}


