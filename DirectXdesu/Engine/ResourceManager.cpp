#include "ResourceManager.h"

ResourceManager* ResourceManager::GetInstance() {
	static ResourceManager instance;

	return &instance;
}

void ResourceManager::Init() {
	models_["Player"] = std::make_unique<MtlObj>("BattleShip");
	models_["MobEnemy1"] = std::make_unique<MtlObj>("MobEnemy1");
	models_["P_Bullet"] = std::make_unique<MtlObj>("playerBullet");
	models_["E_Bullet"] = std::make_unique<MtlObj>("EnemyBullet");
	models_["Explotion"] = std::make_unique<MtlObj>("Explosion");

	models_["Ground"] = std::make_unique<MtlObj>("Grand");
	models_["Bill1"] = std::make_unique<MtlObj>("bill1");

	models_["SkyDorm"] = std::make_unique<MtlObj>("SkyBox");
}

KModel* ResourceManager::GetModels(std::string mapName) { return models_[mapName].get(); }
