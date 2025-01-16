#include "ModelManager.h"

ModelManager* ModelManager::GetInstance() {
	static ModelManager instance;

	return &instance;
}

void ModelManager::Init() {
	models_["S_SkyDorm"] = std::make_unique<MtlObj>("skydome");
	models_["S_Cube"] = std::make_unique<MtlObj>("cube");
	models_["S_Arrow"] = std::make_unique<MtlObj>("arrow");
}

KModel* ModelManager::GetModels(std::string mapName) { return models_[mapName].get(); }
