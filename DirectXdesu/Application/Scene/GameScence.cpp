#include "GameScence.h"
#include "KInput.h"

#include "MyMath.h"

#include "DebugCamera.h"
#include "GameCamera.h"
#include "RailCamera.h"

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
	// テクスチャ
	mario.CreateTexture("Resources/texture/", "mario.jpg");
	haikei.CreateTexture("Resources/texture/", "haikei.jpg");

	// サウンド
	sound = std::make_unique<Sound>();
	sound->Init();
	soundData1 = sound->SoundLoadWave("Resources/Sound/Alarm01.wav");
	soundData2 = sound->SoundLoadWave("Resources/Sound/Alarm02.wav");
	soundData3 = sound->SoundLoadWave("Resources/Sound/Alarm03.wav");
}

void GameScence::Init()
{
	input = KInput::GetInstance();

	// Obj
	objShader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	objPipeline = std::make_unique<KGPlin>();
	objPipeline->CreatePipelineAll(objShader, "Obj");

	// Sprite
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline = std::make_unique<KGPlin>();
	spritePipeline->CreatePipelineAll(spriteShader, "Sprite");

#pragma region スプライト
	sprite = new Sprite();
	sprite->Init();
	sprite->SetPipeline(spritePipeline.get());
#pragma endregion
#pragma endregion

	isDebug = true;
	camera = std::make_unique<RailCamera>();

	sceneManager = SceneManager::GetInstance();

	// プレイヤー
	player = std::make_unique<Player>();
	player->Init();
	const WorldTransfom& cameraToPlayer = camera->GetTransform();
	player->SetParent(&cameraToPlayer);

	// 雑魚敵
	for (size_t i = 0; i < mobEnemy.size(); i++)
	{
		mobEnemy[i] = std::make_unique<MobEnemy>();
		mobEnemy[i]->Init();
	}

	// 地面
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
		sound->SoundPlayWave(soundData1);
	}
	if (ImGui::Button("Play2"))
	{
		sound->SoundPlayWave(soundData2);
	}
	if (ImGui::Button("Play3"))
	{
		sound->SoundPlayWave(soundData3);
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
	ImGui::Text("Flip");
	if (ImGui::Button("FlipX"))
	{
		if (spriteFlipX)
		{
			spriteFlipX = false;
		}
		else
		{
			spriteFlipX = true;
		}
	}
	if (ImGui::Button("FlipY"))
	{
		if (spriteFlipY)
		{
			spriteFlipY = false;
		}
		else
		{
			spriteFlipY = true;
		}
	}

	ImGui::Begin("Player");
	ImGui::Text("pos: (%.2f,%.2f, %.2f)", player->GetPosition().x, player->GetPosition().y, player->GetPosition().z);
	ImGui::End();

	ImGui::Begin("Camera");
	ImGui::Text("pos: (%.2f,%.2f, %.2f)", camera->GetPos().x, camera->GetPos().y, camera->GetPos().z);
	ImGui::End();

	player->Update(camera->GetViewPro());

	for (size_t i = 0; i < mobEnemy.size(); i++)
	{
		mobEnemy[i]->Update(camera->GetViewPro());
	}

	ground->Update(camera->GetViewPro());

	camera->Update();
}

void GameScence::Draw()
{
	// 地面描画
	ground->Draw();

	for (size_t i = 0; i < mobEnemy.size(); i++)
	{
		mobEnemy[i]->Draw();
	}

	// プレイヤー描画
	player->Draw();

	sprite->Draw(&mario,spritePos, spriteSize,spriteRot, spriteColor, spriteFlipX, spriteFlipY);
}

void GameScence::Final()
{
	sound->GetxAudio().Reset();
	sound->SoundUnLoad(&soundData1);
	sound->SoundUnLoad(&soundData2);
	sound->SoundUnLoad(&soundData3);
}

void GameScence::CheckAllCollisions()
{
	// 判定対象AとBの座標
	KMyMath::Vector3 posA, posB;

	// 自機弾の取得
}
