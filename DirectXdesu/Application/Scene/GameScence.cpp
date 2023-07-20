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

GameScence::~GameScence()
{
	Final();
};

void GameScence::LoadResources()
{
	// �e�N�X�`��
	textureData = TextureManager::Load("Resources/texture/mario.jpg");

	// ���f��
	playerModel = std::make_unique<MtlObj>("kariBattle");
	playerModel->CreateModel();
	mobEnemysModel = std::make_unique<MtlObj>("boss_model");
	mobEnemysModel->CreateModel();

	// �T�E���h
	soundData1 = Sound::GetInstance()->SoundLoadWave("Resources/Sound/Alarm01.wav");
	soundData2 = Sound::GetInstance()->SoundLoadWave("Resources/Sound/Alarm02.wav");
	soundData3 = Sound::GetInstance()->SoundLoadWave("Resources/Sound/Alarm03.wav");

	// �p�C�v���C��
	// Obj
	objShader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	objPipeline = std::make_unique<KGPlin>();
	objPipeline->CreatePipelineAll(objShader, "Obj");

	// Sprite
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline = std::make_unique<KGPlin>();
	spritePipeline->CreatePipelineAll(spriteShader, "Sprite");
}

void GameScence::Init()
{
	input = KInput::GetInstance();

#pragma region �X�v���C�g
	sprite = std::make_unique<Sprite>();
	sprite->Init();
	sprite->SetPipeline(spritePipeline.get());
#pragma endregion

	isDebug = true;
	camera = std::make_unique<RailCamera>();

	sceneManager = SceneManager::GetInstance();

	// �v���C���[
	player = std::make_unique<Player>();
	player->Init(playerModel.get());
	const WorldTransfom& cameraToPlayer = camera->GetTransform();
	player->SetParent(&cameraToPlayer);

	// �G���G�o���p�^�[���ǂݍ���
	LoadEnemyPopData();

	// �n��
	ground = std::make_unique<Ground>();
	ground->Init();
}

void GameScence::Update()
{
	if (input->IsTrigger(DIK_SPACE))
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	ImGui::Text("Sound");
	if (ImGui::Button("Play1"))
	{
		Sound::GetInstance()->SoundPlayWave(soundData1);
	}
	if (ImGui::Button("Play2"))
	{
		Sound::GetInstance()->SoundPlayWave(soundData2);
	}
	if (ImGui::Button("Play3"))
	{
		Sound::GetInstance()->SoundPlayWave(soundData3);
	}

	ImGui::Text("Sprite");
	ImGui::Text("pos: (%.2f,%.2f)", spritePos.x, spritePos.y);
	ImGui::SliderFloat("SpritePosX", &spritePos.x, 0.0f, 100.0f);
	ImGui::SliderFloat("SpritePosY", &spritePos.y, 0.0f, 100.0f);
	ImGui::Text("size: (%.2f,%.2f)", spriteSize.x, spriteSize.y);
	ImGui::SliderFloat("SpriteSizeX", &spriteSize.x, 0.0f, 100.0f);
	ImGui::SliderFloat("SpriteSizeY", &spriteSize.y, 0.0f, 100.0f);
	ImGui::Text("rot: (%.2f)", spriteRot);
	ImGui::SliderFloat("SpriteRot", &spriteRot, 0.0f, 180.0f);
	ImGui::Text("color: (%.2f,%.2f,%.2f,%.2f)", spriteColor.x, spriteColor.y, spriteColor.z, spriteColor.w);
	ImGui::SliderFloat("SpriteColorR", &spriteColor.x, 0.0f, 1.0f);
	ImGui::SliderFloat("SpriteColorG", &spriteColor.y, 0.0f, 1.0f);
	ImGui::SliderFloat("SpriteColorB", &spriteColor.z, 0.0f, 1.0f);
	ImGui::SliderFloat("SpriteColorA", &spriteColor.w, 0.0f, 1.0f);
	ImGui::Text("CameraAdvance");
	if (ImGui::Button("ON"))
	{
		camera->SetIsAdvance(true);
	}
	if (ImGui::Button("OFF"))
	{
		camera->SetIsAdvance(false);
	}

	ImGui::Begin("Player");
	ImGui::Text("pos: (%.2f,%.2f, %.2f)", player->GetPosition().x,
		player->GetPosition().y, player->GetPosition().z);
	ImGui::End();

	ImGui::Begin("Camera");
	ImGui::Text("pos: (%.2f,%.2f, %.2f)", camera->GetPos().x, camera->GetPos().y, camera->GetPos().z);
	ImGui::End();

	// �G�o��
	UpdateEnemyPopCommands();

	// �G����
	mobEnemys.remove_if([](std::unique_ptr<MobEnemy>& MobEnemy)
		{
			return MobEnemy->GetIsDead();
		});

	// �����蔻��
	CheckAllCollisions();

	player->Update(camera->GetViewPro());

	for (std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys)
	{
		mobEnemy->Update(camera->GetViewPro(), camera->GetPos());
	}

	ground->Update(camera->GetViewPro());

	camera->Update(player.get());
}

void GameScence::Draw()
{
	// �n�ʕ`��
	ground->Draw();

	for (std::unique_ptr<MobEnemy>& mobEnemy : mobEnemys)
	{
		mobEnemy->Draw();
	}

	// �v���C���[�`��
	player->Draw();

	sprite->Draw(textureData,spritePos, spriteSize,spriteRot, spriteColor, spriteFlipX, spriteFlipY);
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
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();

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
			std::unique_ptr<MobEnemy> newMEnemy = std::make_unique<MobEnemy>();
			// ������
			newMEnemy->Init(mobEnemysModel.get());
			// �Z�b�g
			KMyMath::Vector3 pos = { x,y,z };
			newMEnemy->Set(pos);
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
