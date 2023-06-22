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
	objPipeline->CreatePipeline(objShader, 5);

	// Sprite
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline = std::make_unique<KGPlin>();
	spritePipeline->CreatePipelineAll(spriteShader, false, true);

#pragma region スプライト
	sprite = new Sprite();
	sprite->Init();
	sprite->SetPipeline(spritePipeline.get());
#pragma endregion
#pragma endregion

	isDebug = true;
	camera = std::make_unique<DebugCamera>();

	// ポストエフェクトテスト
	postEffect = std::make_unique<PostEffect>();
	postEffect->Init();
	postEffect->SetPipeline(spritePipeline.get());

	sceneManager = SceneManager::GetInstance();

	// プレイヤー
	player = std::make_unique<Player>();
	player->Init();
}

void GameScence::Update()
{
	if (input->IsTrigger(DIK_SPACE))
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	char bufD[255] = "DebugCamera";
	char bufG[255] = "GameCamera";
	ImGui::Text("CameraMode");
	if (ImGui::Button("ChangeCamera"))
	{
		if (isDebug)
		{
			isDebug = false;
			camera.release();
			camera = std::make_unique<GameCamera>();
		}
		else
		{
			isDebug = true;
			camera.release();
			camera = std::make_unique<DebugCamera>();
		}
	}
	if (isDebug)
	{
		ImGui::InputText("mode", bufD, IM_ARRAYSIZE(bufD));
	}
	else
	{
		ImGui::InputText("mode", bufG, IM_ARRAYSIZE(bufG));
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

	player->Update(camera->viewProjection);

	camera->Update();
}

void GameScence::Draw()
{
	player->Draw();

	sprite->Draw(&mario,spritePos, spriteSize,spriteRot, spriteColor, spriteFlipX, spriteFlipY);

	postEffect->Draw(&mario, { 200,200 }, {200,200});
}

void GameScence::Final()
{
	sound->GetxAudio().Reset();
	sound->SoundUnLoad(&soundData1);
	sound->SoundUnLoad(&soundData2);
	sound->SoundUnLoad(&soundData3);
}
