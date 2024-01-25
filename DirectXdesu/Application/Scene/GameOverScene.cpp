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
	// パイプライン
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline.reset(KGPlin::Create(spriteShader, "Sprite"));

	// ゲームオーバーテクスチャ
	overTex = TextureManager::Load("Resources/texture/kariGameOver.png");

	// プッシュAテクスチャ
	pushATex = TextureManager::Load("Resources/texture/kariNextScene.png");
}

void GameOverScene::Init()
{
	BaseScene::Init();

	input = KInput::GetInstance();

	camera = std::make_unique<DebugCamera>();

	sceneManager = SceneManager::GetInstance();

	// タイトル名
	over.reset(Sprite::Create(spritePipeline.get()));

	// プッシュA
	pushA.reset(Sprite::Create(spritePipeline.get()));
}

void GameOverScene::Update()
{
	camera->Update();

	// 次のシーンへ
	if (!sceneChange->GetIsEffect())
	{
		if (input->IsTrigger(DIK_SPACE) || input->GetPadButtonDown(XINPUT_GAMEPAD_A))
		{
			sceneChange->SceneChangeStart();
		}
	}

	if (sceneChange->GetIsChange())
	{
		// シーン切り替え依頼
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
