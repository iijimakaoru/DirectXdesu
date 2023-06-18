#pragma once
#include "FbxLoader.h"
#include "KWinApp.h"
#include "KDirectXCommon.h"
#include "KInput.h"
#include "SceneManager.h"
#include "ImguiManager.h"
#include "ASceneFactory.h"

class Framework
{
public:
	// ������
	virtual void Init();
	// �I��
	virtual void Final();
	// �X�V
	virtual void Update();
	// �`��
	virtual void Draw() = 0;
	// �I���`�F�b�N
	virtual bool IsEndRwquest();

	virtual ~Framework() = default;

	// ���s
	void Run();

protected:
	FbxLoader* fbxLoader = nullptr;

	KWinApp* win = nullptr;

	KDirectXCommon* dx = nullptr;

	KInput* input = nullptr;

	SceneManager* sceneManager = nullptr;

	// �V�[���t�@�N�g���[
	ASceneFactory* sceneFactory = nullptr;

	ImguiManager imguiMane;
};

