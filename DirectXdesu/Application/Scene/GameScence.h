#pragma once
#include "KDirectXCommon.h"
#include "KObject3d.h"
#include "Sprite.h"
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

#include "Warning.h"

#include "BossEnemy.h"

#include "BulletManager.h"

#include "SkyBox.h"

#include "ParticleManager.h"

class GameScence : public BaseScene
{
public:
	GameScence(){};
	~GameScence();
	void LoadResources() override;
	void Init() override;
	void Update() override;
	void ObjDraw() override;
	void SpriteDraw() override;
	void Final()override;

private:
	// �Փ˔���
	void CheckAllCollisions();

	// �G�����f�[�^�̓ǂݍ���
	void LoadEnemyPopData();

	// �G�����R�}���h�̍X�V
	void UpdateEnemyPopCommands();

	// �{�X�o�g���J�n
	void BossBattleStart();

private:
	// �C���v�b�g
	KInput* input = nullptr;

	// �J����
	std::unique_ptr<RailCamera> camera = nullptr;

	// �p�C�v���C��
	std::unique_ptr<KGPlin> objPipeline;
	std::unique_ptr<KGPlin> spritePipeline;
	std::unique_ptr<KGPlin> fbxPipeline;

	// �V�F�[�_�[
	KShader objShader;
	KShader spriteShader;
	KShader fbxShader;

	// �T�E���h
	SoundData soundData1;
	SoundData soundData2;
	SoundData soundData3;

	// �v���C���[
	std::unique_ptr<Player> player = nullptr;
	std::unique_ptr<KModel> playerModel;

	// �v���C���[�̒e���f��
	std::unique_ptr<KModel> playersBulletModel = nullptr;

	// �G��
	std::list<std::unique_ptr<MobEnemy>> mobEnemys;
	std::unique_ptr<KModel> mobEnemysModel;

	// �G�̒e���f��
	std::unique_ptr<KModel> enemysBulletModel = nullptr;

	// �ȈՒn��
	std::unique_ptr<Ground> ground = nullptr;

	// �G�����R�}���h
	std::stringstream enemyPopCommands;
	bool isStand = false;
	int32_t waitTimer = 0;

	TextureData textureData;
	TextureData textureData2;

	// �{�X�o�g��
	std::unique_ptr<BossEnemy> boss = nullptr;

	// �o��x��
	std::unique_ptr<Warning> bossWarning = nullptr;
	bool isBossBattle = false;

	// �o���b�g�}�l�[�W���[
	BulletManager* bulletManager = nullptr;

	// �X�J�C�{�b�N�X
	std::unique_ptr<SkyBox> skyBox = nullptr;
	std::unique_ptr<KModel> skyBoxModel = nullptr;

	// �p�[�e�B�N��
	ParticleManager* particleManager = nullptr;

	// �ʂ̃V�[���ւ̈ڍs�^�C�}�[
	// �Q�[���N���A
	float goClearSceneTime = 120;
	float goClearSceneTimer = 0;

	// �Q�[���I�[�o�[
	float goOverSceneTime = 120;
	float goOverSceneTimer = 0;
};

