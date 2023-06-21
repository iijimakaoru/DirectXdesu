#pragma once
#include "KDirectXCommon.h"
#include "KObject3d.h"
#include "Sprite.h"
#include "KTexture.h"
#include "Sound.h"
#include "KGPlin.h"
#include "KShader.h"
#include <imgui.h>

#include "Camera.h"

#include <array>

#include "FbxObject3D.h"

#include "KModel.h"

#include "BaseScene.h"

#include "KInput.h"

#include "PostEffect.h"

class GameScence : public BaseScene
{
public:
	enum class CollisionMode
	{
		Sphere_Plane,
		Sphere_Triangle,
		Ray_Plane,
		Ray_Triangle,
	};

	GameScence()
	{
		LoadResources();
		Init();
	};
	~GameScence();
	void LoadResources() override;
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
	std::unique_ptr<KGPlin> objPipeline;
	std::unique_ptr<KGPlin> fbxPipeline;
	std::unique_ptr<KGPlin> spritePipeline;

	// �V�F�[�_�[
	KShader objShader;
	KShader fbxShader;
	KShader spriteShader;

	// ���f��
	std::unique_ptr<KModel> SkyBox;

	// �e�N�X�`��
	KTexture mario;
	KTexture haikei;

	// 3D�I�u�W�F�N�g
	std::unique_ptr<KObject3d> skydorm;

	KMyMath::Vector3 center = { 0,0,1 };

	float rSpeed = -0.02f;
	float gSpeed = 0.02f;
	float bSpeed = -0.02f;
	float aSpeed = -0.02f;

	std::unique_ptr<Sound> sound;

	SoundData soundData1;
	SoundData soundData2;
	SoundData soundData3;

	bool isDebug = true;

	// Fbx�֘A
	FbxModel* fbxModel1 = nullptr;
	FbxObject3D* object1 = nullptr;

	// �X�v���C�g
	Sprite* sprite = nullptr;
	KMyMath::Vector2 spritePos = { 0,0 };
	KMyMath::Vector2 spriteSize = { 100,100 };
	float spriteRot = 0;
	KMyMath::Vector4 spriteColor = { 1.0f,1.0f,1.0f,1.0f };
	bool spriteFlipX, spriteFlipY = false;

	// �|�X�g�G�t�F�N�g
	std::unique_ptr<PostEffect> postEffect = nullptr;
};

