#pragma once
#include "Camera.h"
#include<memory>

class SceneManager;

class BaseScene
{
public: // �����o�֐�
	virtual ~BaseScene() = default;

	// ������
	virtual void Init() = 0;

	// �X�V
	virtual void Update() = 0;

	// �`��
	virtual void Draw() = 0;

	// �I��
	virtual void Final() = 0;

	// ���\�[�X�ǂݍ���
	virtual void LoadResources() = 0;
	
	/// <summary>
	/// �V�[���}�l�[�W���[�Z�b�g
	/// </summary>
	/// <param name="sceneManager_"></param>
	virtual void SetSceneManager(SceneManager* sceneManager_)
	{
		sceneManager = sceneManager_;
	}

protected:
	// �V�[���}�l�[�W���[���؂�Ă���
	SceneManager* sceneManager = nullptr;
};
