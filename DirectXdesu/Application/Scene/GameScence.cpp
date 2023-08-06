#include "GameScence.h"
#include "KInput.h"

#include "MyMath.h"

#include "DebugCamera.h"
#include "GameCamera.h"

#include "Collision.h"
#include <sstream>
#include <iomanip>

#include "FbxLoader.h"

#include "SceneManager.h"

#include "Blaster.h"

#include "AppearEnemy.h"

GameScence::~GameScence()
{
	Final();
};

void GameScence::LoadResources()
{
	// �e�N�X�`��
	textureData = TextureManager::Load("Resources/texture/mario.jpg");
	textureData2 = TextureManager::Load("Resources/texture/kariPlayerColor.png");

	// ���f��
	playerModel = std::make_unique<MtlObj>("BattleShip");
	playerModel->CreateModel();
	mobEnemysModel = std::make_unique<MtlObj>("boss_model");
	mobEnemysModel->CreateModel();
	playersBulletModel = std::make_unique<MtlObj>("playerBullet");
	playersBulletModel->CreateModel();
	skyBoxModel = std::make_unique<MtlObj>("SkyBox");
	skyBoxModel->CreateModel();

	// �T�E���h
	soundData1 = Sound::GetInstance()->SoundLoadWave("Resources/Sound/Alarm01.wav");
	soundData2 = Sound::GetInstance()->SoundLoadWave("Resources/Sound/Alarm02.wav");
	soundData3 = Sound::GetInstance()->SoundLoadWave("Resources/Sound/Alarm03.wav");

	// �p�C�v���C��
	// Obj
	objShader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	objPipeline.reset(KGPlin::Create(objShader, "Obj"));

	// Sprite
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline.reset(KGPlin::Create(spriteShader, "Sprite"));

	// fbx
	fbxShader.Init(L"Resources/Shader/FbxVS.hlsl", L"Resources/Shader/FbxPS.hlsl");
	fbxPipeline.reset(KGPlin::Create(fbxShader, "Fbx"));
}

void GameScence::Init()
{
	// �C���X�^���X
	input = KInput::GetInstance();

	camera = std::make_unique<RailCamera>();

	sceneManager = SceneManager::GetInstance();

	// �v���C���[
	player.reset(Player::Create(playerModel.get(), objPipeline.get(), 10, spritePipeline.get()));
	player->SetParent(&camera->GetTransform());

	// �G���G�o���p�^�[���ǂݍ���
	LoadEnemyPopData();

	// �n��
	ground = std::make_unique<Ground>();
	ground->Init();

	// �X�J�C�{�b�N�X
	skyBox.reset(SkyBox::Create(skyBoxModel.get(),objPipeline.get(),player->GetWorldPos().z));

	bulletManager = BulletManager::GetInstance();
	bulletManager->Init(playersBulletModel.get(), objPipeline.get());

	// �p�[�e�B�N��
	particleManager = ParticleManager::GetInstance();
	particleManager->Init();
}

void GameScence::Update()
{
	ImGui::Begin("Camera");
	ImGui::Text("CameraPos(%f,%f,%f)", camera->GetPos().x, camera->GetPos().y, camera->GetPos().z);
	ImGui::End();

	ImGui::Begin("Ground");
	ImGui::Text("Grand1Pos(%f,%f,%f)", ground->GetPos(0).x, ground->GetPos(0).y, ground->GetPos(0).z);
	ImGui::Text("Grand2Pos(%f,%f,%f)", ground->GetPos(1).x, ground->GetPos(1).y, ground->GetPos(1).z);
	ImGui::End();

	if (input->IsTrigger(DIK_0))
	{
		player->OnCollision();
	}

	// �{�X�o�g���J�n����
	BossBattleStart();

	// �G�o��
	UpdateEnemyPopCommands();

	// �����蔻��
	CheckAllCollisions();

	if (input->IsTrigger(DIK_SPACE))
	{
		sceneManager->ChangeScene("TITLE");
	}

	// �G����
	mobEnemys.remove_if([](std::unique_ptr<MobEnemy>& MobEnemy)
		{
			return MobEnemy->GetIsDead();
		});

	// �v���C���[�̍X�V
	player->Update(camera->GetViewPro());

	// �G���G�̍X�V
	for (std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys)
	{
		mobEnemy->Update(camera->GetViewPro(), camera->GetPos());
	}

	// �{�X�̍X�V
	if (boss)
	{
		boss->Update(camera->GetViewPro());
	}

	// �e�̍X�V
	bulletManager->Update(camera->GetViewPro());

	// �n�ʂ̍X�V
	ground->Update(camera->GetViewPro(), camera->GetPos());

	// �X�J�C�{�b�N�X�̍X�V
	skyBox->SetPosZ(player->GetWorldPos().z);
	skyBox->Update(camera->GetViewPro());

	// �p�[�e�B�N���}�l�[�W���[�̍X�V
	particleManager->Update(camera->GetViewPro());

	// �J�����̍X�V
	camera->Update(player.get());

	// �{�X�o��x��
	if (bossWarning)
	{
		bossWarning->Update();
	}
}

void GameScence::ObjDraw()
{
	// �n�ʕ`��
	ground->Draw();

	// �G���G�`��
	for (std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys)
	{
		mobEnemy->Draw();
	}

	// �{�X�`��
	if (boss)
	{
		boss->Draw();
	}

	// �v���C���[�`��
	player->ObjDraw();

	skyBox->ObjDraw();

	bulletManager->Draw();

	particleManager->Draw();
}

void GameScence::SpriteDraw()
{
	player->SpriteDraw();

	player->UIDraw();

	// �{�X�o��x�����o
	if (bossWarning)
	{
		bossWarning->Draw();
	}
}

void GameScence::Final()
{
	Sound::GetInstance()->GetxAudio().Reset();
	Sound::GetInstance()->SoundUnLoad(&soundData1);
	Sound::GetInstance()->SoundUnLoad(&soundData2);
	Sound::GetInstance()->SoundUnLoad(&soundData3);
}

void GameScence::CheckAllCollisions()
{
	// ����Ώ�A��B�̍��W
	KMyMath::Vector3 posA, posB;

	// ���@�e�̎擾
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = bulletManager->GetPlayerBullets();

	// �G�e�̎擾
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = bulletManager->GetEnemyBullets();

	// ���e�ƓG�̓����蔻��
	{
		for (std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys)
		{
			// �G�̍��W
			posA = mobEnemy->GetWorldPos();

			for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
			{
				posB = bullet->GetWorldPos();

				// �����m�̌�������
				if (MyCollisions::CheckSphereToSphere(posA, posB, 6, 2))
				{
					// �e����
					bullet->OnCollision();

					// �G����
					mobEnemy->OnCollision();
				}
			}
		}
	}

	// �G�e�Ǝ��@�̓����蔻��
	{
		// ���@�̍��W
		posA = player->GetWorldPos();

		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
		{
			posB = bullet->GetWorldPos();

			// �����m�̌�������
			if (MyCollisions::CheckSphereToSphere(posA, posB, 3, 2))
			{
				// �e����
				bullet->OnCollision();

				// ���@��e����
				player->OnCollision();
			}
		}
	}
}

void GameScence::LoadEnemyPopData()
{
	// �t�@�C�����J��
	std::ifstream file;
	file.open("Resources/csv/enemyPop.csv");
	assert(file.is_open());

	// �t�@�C���̓��e�𕶎��X�g���[���ɃR�s�[
	enemyPopCommands << file.rdbuf();

	// �t�@�C�������
	file.close();
}

void GameScence::UpdateEnemyPopCommands()
{
	// �ҋ@����
	if (isStand)
	{
		waitTimer--;
		if (waitTimer <= 0)
		{
			// �ҋ@�I��
			isStand = false;
		}
		return;
	}

	// 1�s���̕����������ϐ�
	std::string line;

	// �R�}���h���s���[�v
	while (getline(enemyPopCommands, line))
	{
		// 1�s���̕��������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		std::string word;
		// ,��؂�ōs�̐擪��������擾
		getline(line_stream, word, ',');

		// �R�����g
		if (word.find("//") == 0)
		{
			// �s���΂�
			continue;
		}

		// POP
		if (word.find("POP") == 0)
		{
			// x���W
			getline(line_stream, word, ',');
			float x = static_cast<float>(std::atof(word.c_str()));

			// y���W
			getline(line_stream, word, ',');
			float y = static_cast<float>(std::atof(word.c_str()));

			// z���W
			getline(line_stream, word, ',');
			float z = static_cast<float>(std::atof(word.c_str()));

			/// �G�𔭐�������
			// ����
			std::unique_ptr<MobEnemy> newMEnemy;
			newMEnemy.reset(AppearEnemy::Create(mobEnemysModel.get(), objPipeline.get(), { x,y,z }));
			newMEnemy->SetPlayer(player.get());
			// �o�^
			mobEnemys.push_back(std::move(newMEnemy));
		}
		// WAIT�R�}���h
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');

			// �҂�����
			int32_t waitTime = atoi(word.c_str());

			// �ҋ@�J�n
			isStand = true;
			waitTimer = waitTime;

			// �R�}���h���[�v�𔲂���
			break;
		}
	}
}

void GameScence::BossBattleStart()
{
	// �{�X�o�g�����n�܂��Ă�΃X�L�b�v
	if (isBossBattle) { return; }

	// �{�X�o�g���J�n���W
	const float bossBattleStartPos = 3200;

	if (!bossWarning)
	{
		bool isBossBattleStart = camera->GetPos().z >= bossBattleStartPos;
		if (!isBossBattleStart) { return; }

		// �J�����O�i�~�߂�
		camera->SetIsAdvance(false);

		// �{�X�o��x���쐬
		bossWarning = std::make_unique<Warning>();
		bossWarning->Init();
	}
	else
	{
		// ���o���I����ĂȂ��Ƃ��͔�����
		if (!bossWarning->GetIsDelete()) { return; }

		// �{�X�o��x�����
		bossWarning.reset();

		// �{�X�z�u
		const float bossDistance = 150;
		const KMyMath::Vector3 bossBasePos = { 0.0f, 23.0f, bossBattleStartPos + bossDistance };
		boss.reset(Blaster::Create(mobEnemysModel.get(), objPipeline.get(), bossBasePos));

		// �{�X�o�g���J�n
		isBossBattle = true;
	}
}
