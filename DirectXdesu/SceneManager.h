#pragma once
#include "BaseScene.h"
#include "ASceneFactory.h"

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
	/// ���̃V�[��
	/// </summary>
	/// <param name="sceneName"></param>
	void ChangeScene(const std::string& sceneName);

	/// <summary>
	/// �V�[���t�@�N�g���[�̃Z�b�^�[
	/// </summary>
	/// <param name="sceneFactory_"></param>
	void SetSceneFactory(ASceneFactory* sceneFactory_)
	{
		sceneFactory = sceneFactory_;
	}

	static SceneManager* GetInstance();

private:
	// ���̃V�[��
	BaseScene* scene = nullptr;
	// ���̃V�[��
	BaseScene* nextScene = nullptr;

	// �V�[���t�@�N�g���[
	ASceneFactory* sceneFactory = nullptr;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	const SceneManager& operator=(const SceneManager&) = delete;
};

