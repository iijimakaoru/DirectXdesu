#include "ModelManager.h"

ModelManager* ModelManager::GetInstance() {
	static ModelManager instance;

	return &instance;
}

void ModelManager::Init() {
	models_["Player"] = std::make_unique<MtlObj>("BattleShip");
	models_["MobEnemy1"] = std::make_unique<MtlObj>("MobEnemy1");
	models_["P_Bullet"] = std::make_unique<MtlObj>("playerBullet");
	models_["E_Bullet"] = std::make_unique<MtlObj>("EnemyBullet");
	models_["Explotion"] = std::make_unique<MtlObj>("Explosion");
	models_["Blaster_Core"] = std::make_unique<MtlObj>("BlasterCore");
	models_["Blaster_Unit"] = std::make_unique<MtlObj>("BlasterUnit");
	models_["Lazer"] = std::make_unique<MtlObj>("Lazer");

	models_["Ground"] = std::make_unique<MtlObj>("Grand");
	models_["Bill1"] = std::make_unique<MtlObj>("bill1");

	models_["SkyDorm"] = std::make_unique<MtlObj>("SkyBox");
	models_["T_SkyDorm"] = std::make_unique<MtlObj>("Hosizora");

	models_["Cube"] = std::make_unique<Cube>();
}

KModel* ModelManager::GetModels(std::string mapName) { return models_[mapName].get(); }
