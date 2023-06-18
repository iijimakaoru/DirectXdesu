#pragma once
#include "BaseScene.h"
#include <string>

class ASceneFactory
{
public:
	// 仮想デストラクタ
	virtual ~ASceneFactory() = default;
	// シーン生成
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
};

