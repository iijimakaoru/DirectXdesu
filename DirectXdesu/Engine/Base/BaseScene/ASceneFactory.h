#pragma once
#include "BaseScene.h"
#include <string>

class ASceneFactory
{
public:
	// ���z�f�X�g���N�^
	virtual ~ASceneFactory() = default;
	// �V�[������
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
};

