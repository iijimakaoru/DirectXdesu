#pragma once
#include "ASceneFactory.h"

// �V�[���H��
class SceneFactory : public ASceneFactory
{
public:
	/// <summary>
	/// �V�[���̐���
	/// </summary>
	/// <param name="sceneName"></param>
	/// <returns></returns>
	BaseScene* CreateScene(const std::string& sceneName) override;
};

