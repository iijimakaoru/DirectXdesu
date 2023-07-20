#include "TitleScene.h"
#include "FbxLoader.h"

#include "DebugCamera.h"

#include "SceneManager.h"

#include <imgui.h>

TitleScene::~TitleScene()
{
	Final();
}

void TitleScene::LoadResources()
{
	// �p�C�v���C��
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline = std::make_unique<KGPlin>();
	spritePipeline->CreatePipelineAll(spriteShader, "Sprite");

	//// �^�C�g�����e�N�X�`��
	//titleTex.CreateTexture("Resources/texture/", "kariTitle.png");
}

void TitleScene::Init()
{
	input = KInput::GetInstance();

	camera = std::make_unique<DebugCamera>();

	sceneManager = SceneManager::GetInstance();

	//// �^�C�g����
	//titleName = std::make_unique<Sprite>();
	//titleName->Init();
	//titleName->SetPipeline(spritePipeline.get());
}

void TitleScene::Update()
{
	camera->Update();

	// ���̃V�[����
	if (input->IsTrigger(DIK_SPACE))
	{
		// �V�[���؂�ւ��˗�
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void TitleScene::Draw()
{
	/*titleName->Draw(&titleTex, { static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW()) / 2,
		static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH()) / 2 },{500,120});*/
}

void TitleScene::Final()
{
	
}
