#include "GameOverScene.h"

#include "DebugCamera.h"

#include "SceneManager.h"

#include <imgui.h>

GameOverScene::~GameOverScene()
{
	Final();
}

void GameOverScene::LoadResources()
{
	// �p�C�v���C��
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline = std::make_unique<KGPlin>();
	spritePipeline->CreatePipelineAll(spriteShader, "Sprite");

	// �Q�[���I�[�o�[�e�N�X�`��
	overTex = TextureManager::Load("Resources/texture/kariGameOver.png");

	// �v�b�V��A�e�N�X�`��
	pushATex = TextureManager::Load("Resources/texture/kariNextScene.png");
}

void GameOverScene::Init()
{
	BaseScene::Init();

	input = KInput::GetInstance();

	camera = std::make_unique<DebugCamera>();

	sceneManager = SceneManager::GetInstance();

	// �^�C�g����
	over = std::make_unique<Sprite>();
	over->Init();
	over->SetPipeline(spritePipeline.get());

	// �v�b�V��A
	pushA = std::make_unique<Sprite>();
	pushA->Init();
	pushA->SetPipeline(spritePipeline.get());
}

void GameOverScene::Update()
{
	camera->Update();

	// ���̃V�[����
	if (!sceneChange->GetIsEffect())
	{
		if (input->IsTrigger(DIK_SPACE) || input->GetPadButtonDown(XINPUT_GAMEPAD_A))
		{
			sceneChange->Start();
		}
	}

	if (sceneChange->GetIsChange())
	{
		// �V�[���؂�ւ��˗�
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameOverScene::ObjDraw()
{
}

void GameOverScene::SpriteDraw()
{
	const float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	const float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	over->Draw(overTex, { width / 2, height * 1 / 3 }, { 1,1 });

	pushA->Draw(pushATex, { width / 2, height * 2 / 3 }, { 1,1 });
}

void GameOverScene::Final()
{
}
