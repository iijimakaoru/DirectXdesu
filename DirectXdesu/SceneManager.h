#pragma once
#include "BaseScene.h"

class SceneManager
{
public:
	// �X�V
	void Update();

	// �`��
	void Draw();

	// �I��
	void Final();

	/// <summary>
	/// ���̃V�[���\��
	/// </summary>
	/// <param name="nextScene_"></param>
	void SetNestScene(BaseScene* nextScene_)
	{
		nextScene = nextScene_;
	}

	static SceneManager* GetInstance();

private:
	// ���̃V�[��
	BaseScene* scene = nullptr;
	// ���̃V�[��
	BaseScene* nextScene = nullptr;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	const SceneManager& operator=(const SceneManager&) = delete;
};

