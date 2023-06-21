#pragma once
#include "Camera.h"
#include "KGPlin.h"
#include "FbxObject3D.h"

#include "BaseScene.h"

#include "KInput.h"

#include "PostEffect.h"

#include "KTexture.h"

#include "Sprite.h"

class TitleScene : public BaseScene
{
public:
	TitleScene()
	{
		LoadResources();
		Init();
	};
	~TitleScene();
	void LoadResources()override;
	void Init() override;
	void Update() override;
	void Draw() override;
	void Final()override;

private:
	// �C���v�b�g
	KInput* input = nullptr;

	// �J����
	std::unique_ptr<Camera> camera = nullptr;

	// �p�C�v���C��
	std::unique_ptr<KGPlin> fbxPipeline;
	std::unique_ptr<KGPlin> spritePipeline;

	// �V�F�[�_�[
	KShader fbxShader;
	KShader spriteShader;

	// Fbx�֘A
	FbxModel* fbxModel1 = nullptr;
	FbxObject3D* object1 = nullptr;

	// �|�X�g�G�t�F�N�g
	std::unique_ptr<PostEffect> postEffect = nullptr;

	// �X�v���C�g
	Sprite* sprite = nullptr;
	KMyMath::Vector2 spritePos = { 0,0 };
	KMyMath::Vector2 spriteSize = { 100,100 };
	float spriteRot = 0;
	KMyMath::Vector4 spriteColor = { 1.0f,1.0f,1.0f,1.0f };
	bool spriteFlipX, spriteFlipY = false;

	// �e�N�X�`��
	KTexture tex1;
};

