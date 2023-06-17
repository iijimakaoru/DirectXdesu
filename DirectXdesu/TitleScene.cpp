#include "TitleScene.h"
#include "FbxLoader.h"

#include "DebugCamera.h"

#include "GameScence.h"

#include "SceneManager.h"

TitleScene::~TitleScene()
{
	Final();
}

void TitleScene::LoadResources()
{
}

void TitleScene::Init()
{
	input = KInput::GetInstance();

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

	camera = std::make_unique<DebugCamera>();
}

void TitleScene::Update()
{
	// Fbx
	object1->Update(camera->viewProjection);

	// ���̃V�[����
	if (input->IsTrigger(DIK_SPACE))
	{
		// �Q�[���V�[���𐶐�
		BaseScene* scene = new GameScence();
		// �V�[���؂�ւ��˗�
		sceneManager->SetNestScene(scene);
	}
}

void TitleScene::Draw()
{
	object1->Draw();
}

void TitleScene::Final()
{
	delete object1;
	delete fbxModel1;
}
