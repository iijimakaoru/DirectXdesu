#include "GameScence.h"
#include "KInput.h"

#include "MyMath.h"

#include "DebugCamera.h"
#include "GameCamera.h"

#include "Collision.h"
#include <sstream>
#include <iomanip>

#include "FbxLoader.h"

GameScence::~GameScence()
{
	delete camera;
	sound->GetxAudio().Reset();
	sound->SoundUnLoad(&soundData1);
	sound->SoundUnLoad(&soundData2);
	sound->SoundUnLoad(&soundData3);

	delete object1;
	delete fbxModel1;
};

void GameScence::LoadResources()
{
	// モデル
	SkyBox = std::make_unique<MtlObj>("boxSky");
	SkyBox->CreateModel();

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
	// Obj
	objShader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	objPipeline = std::make_unique<KGPlin>();
	objPipeline->CreatePipeline(objShader, 5);

	// Fbx
	fbxShader.Init(L"Resources/Shader/FBXVS.hlsl", L"Resources/Shader/FBXPS.hlsl");
	fbxPipeline = std::make_unique<KGPlin>();
	fbxPipeline->CreatePipelineAll(fbxShader, false, false, false, true);

	// Sprite
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline = std::make_unique<KGPlin>();
	spritePipeline->CreatePipelineAll(spriteShader, false, true);

#pragma region Fbxテスト
	fbxModel1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	object1 = new FbxObject3D;
	object1->Init();
	object1->SetPipline(fbxPipeline.get());
	object1->SetModel(fbxModel1);
	object1->PlayAnimation();
#pragma endregion

	skydorm = std::make_unique<KObject3d>();
	skydorm->Initialize();
	skydorm->LoadModel(SkyBox.get());
	skydorm->SetPipeline(objPipeline.get());
	skydorm->transform.scale = { 500,500,500 };

#pragma region スプライト
	sprite = new Sprite();
	sprite->Init();
	sprite->SetPipeline(spritePipeline.get());
#pragma endregion
#pragma endregion

	isDebug = true;
	camera = new DebugCamera();
}

void GameScence::Update()
{
	char bufD[255] = "DebugCamera";
	char bufG[255] = "GameCamera";
	ImGui::Text("CameraMode");
	if (ImGui::Button("ChangeCamera"))
	{
		if (isDebug)
		{
			isDebug = false;
			delete camera;
			camera = new GameCamera();
		}
		else
		{
			isDebug = true;
			delete camera;
			camera = new DebugCamera();
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

	camera->Update();

	skydorm->Update(camera->viewProjection);

	XMFLOAT3 fbxPos = object1->GetPosition();
	ImGui::Text("Fbx");
	ImGui::Text("pos: (%.2f,%.2f,%.2f)", fbxPos.x, fbxPos.y, fbxPos.z);
	ImGui::SliderFloat("FbxPosX", &fbxPos.x, -60.0f, 60.0f);
	ImGui::SliderFloat("FbxPosY", &fbxPos.y, -60.0f, 60.0f);
	ImGui::SliderFloat("FbxPosZ", &fbxPos.z, -60.0f, 60.0f);

	ImGui::Text("Sprite");
	ImGui::Text("pos: (%.2f,%.2f)", sprite->info.position.x, sprite->info.position.y);
	ImGui::SliderFloat("SpritePosX", &sprite->info.position.x, 0.0f, 1.5f);
	ImGui::SliderFloat("SpritePosY", &sprite->info.position.y, -1.5f, 0.0f);
	ImGui::Text("size: (%.2f,%.2f)", sprite->info.size_.x, sprite->info.size_.y);
	ImGui::SliderFloat("SpriteSizeX", &sprite->info.size_.x, 0.0f, 250.0f);
	ImGui::SliderFloat("SpriteSizeY", &sprite->info.size_.y, 0.0f, 250.0f);
	ImGui::Text("rot: (%.2f)", sprite->info.rotation);
	ImGui::SliderFloat("SpriteRot", &sprite->info.rotation, 0.0f, 180.0f);
	ImGui::Text("anchor: (%.2f,%.2f)", sprite->info.anchorPoint.x, sprite->info.anchorPoint.y);
	ImGui::SliderFloat("SpriteAnchorX", &sprite->info.anchorPoint.x, 0.0f, 1.0f);
	ImGui::SliderFloat("SpriteAnchorY", &sprite->info.anchorPoint.y, 0.0f, 1.0f);
	if (ImGui::Button("FlipX"))
	{
		if (sprite->info.isFlipX)
		{
			sprite->info.isFlipX = false;
		}
		else
		{
			sprite->info.isFlipX = true;
		}
	}
	if (ImGui::Button("FlipY"))
	{
		if (sprite->info.isFlipY)
		{
			sprite->info.isFlipY = false;
		}
		else
		{
			sprite->info.isFlipY = true;
		}
	}
	if (ImGui::Button("Invisible"))
	{
		if (sprite->info.isInvisible)
		{
			sprite->info.isInvisible = false;
		}
		else
		{
			sprite->info.isInvisible = true;
		}
	}

	object1->SetPosition({ fbxPos.x,fbxPos.y,fbxPos.z });

	// Fbx
	object1->Update(camera->viewProjection);

	sprite->Update();
}

void GameScence::Draw()
{
	object1->Draw();

	skydorm->Draw();

	sprite->Draw(&mario);
}
