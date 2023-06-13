#include "GameScence.h"
#include "KInput.h"

#include "ParticleManager.h"
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
	// ���f��
	SkyBox = std::make_unique<MtlObj>("boxSky");
	SkyBox->CreateModel();

	// �e�N�X�`��
	mario.CreateTexture("Resources/texture/", "mario.jpg");
	haikei.CreateTexture("Resources/texture/", "haikei.jpg");

	// �T�E���h
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

#pragma region Fbx�e�X�g
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

#pragma region �X�v���C�g
	sprite = std::make_unique<Sprite>();
	sprite->Init(KDirectXCommon::GetInstance());
	spriteCommon = sprite->SpriteCommonCreate();
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

	object1->SetPosition({ fbxPos.x,fbxPos.y,fbxPos.z });

	// Fbx
	object1->Update(camera->viewProjection);
}

void GameScence::Draw()
{
	object1->Draw();

	skydorm->Draw();
}
