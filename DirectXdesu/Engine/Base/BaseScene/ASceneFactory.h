#pragma once
#include "BaseScene.h"
#include <string>

/**
 * @file ASceneFactory.h
 * @brief シーンファクトリーの基底クラス
 * @author 飯島 薫
 */

class ASceneFactory
{
public:
	// 仮想デストラクタ
	virtual ~ASceneFactory() = default;
	// シーン生成
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
};

