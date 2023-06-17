#pragma once
#include "BaseScene.h"

class SceneManager
{
public:
	~SceneManager();

	// �X�V
	void Update();

	// �`��
	void Draw();

	/// <summary>
	/// ���̃V�[���\��
	/// </summary>
	/// <param name="nextScene_"></param>
	void SetNestScene(BaseScene* nextScene_)
	{
		nextScene = nextScene_;
	}

private:
	// ���̃V�[��
	BaseScene* scene = nullptr;
	// ���̃V�[��
	BaseScene* nextScene = nullptr;
};

