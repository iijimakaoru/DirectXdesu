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

#include "Player.h"

#include "Bullet.h"

#include "MobEnemy.h"

#include "Ground.h"

#include "RailCamera.h"

class GameScence : public BaseScene
{
public:
	GameScence(){};
	~GameScence();
	void LoadResources() override;
	void Init() override;
	void Update() override;
	void Draw() override;
	void Final()override;

private:
	// �Փ˔���
	void CheckAllCollisions();

	// �G�����f�[�^�̓ǂݍ���
	void LoadEnemyPopData();

	// �G�����R�}���h�̍X�V
	void UpdateEnemyPopCommands();

private:
	// �C���v�b�g
	KInput* input = nullptr;

	// �J����
	std::unique_ptr<RailCamera> camera = nullptr;

	// �p�C�v���C��
	std::unique_ptr<KGPlin> objPipeline;
	std::unique_ptr<KGPlin> spritePipeline;

	// �V�F�[�_�[
	KShader objShader;
	KShader spriteShader;

	// �e�N�X�`��
	KTexture mario;
	KTexture haikei;

	SoundData soundData1;
	SoundData soundData2;
	SoundData soundData3;

	bool isDebug = true;

	// �X�v���C�g
	std::unique_ptr<Sprite> sprite = nullptr;
	KMyMath::Vector2 spritePos = { 0,0 };
	KMyMath::Vector2 spriteSize = { 100,100 };
	float spriteRot = 0;
	KMyMath::Vector4 spriteColor = { 1.0f,1.0f,1.0f,1.0f };
	bool spriteFlipX, spriteFlipY = false;

	// �v���C���[
	std::unique_ptr<Player> player = nullptr;
	std::unique_ptr<KModel> playerModel;

	// �G��
	std::list<std::unique_ptr<MobEnemy>> mobEnemys;
	std::unique_ptr<KModel> mobEnemysModel;

	// �ȈՒn��
	std::unique_ptr<Ground> ground = nullptr;

	// �G�����R�}���h
	std::stringstream enemyPopCommands;
	bool isStand = false;
	int32_t waitTimer = 0;
};

