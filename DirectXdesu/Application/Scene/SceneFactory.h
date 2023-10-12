#pragma once
#include "ASceneFactory.h"

/**
 * @file SceneFactory.h
 * @brief シーンファクトリー
 * @author 飯島 薫
 */

// シーン工場
class SceneFactory : public ASceneFactory
{
public:
	/// <summary>
	/// シーンの生成
	/// </summary>
	/// <param name="sceneName"></param>
	/// <returns></returns>
	BaseScene* CreateScene(const std::string& sceneName) override;
};

