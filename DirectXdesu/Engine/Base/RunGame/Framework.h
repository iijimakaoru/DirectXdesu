#pragma once
#include "FbxLoader.h"
#include "KWinApp.h"
#include "KDirectXCommon.h"
#include "KInput.h"
#include "SceneManager.h"
#include "ImguiManager.h"
#include "ASceneFactory.h"
#include "PostEffect.h"
#include "KGPlin.h"
#include "KShader.h"

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

	// �p�C�v���C��
	std::unique_ptr<KGPlin> spritePipeline;

	// �V�F�[�_�[
	KShader spriteShader;

	// �|�X�g�G�t�F�N�g
	std::unique_ptr<PostEffect> postEffect = nullptr;
};

