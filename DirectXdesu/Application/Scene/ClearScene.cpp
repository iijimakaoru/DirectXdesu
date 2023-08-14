#include "ClearScene.h"

#include "DebugCamera.h"

#include "SceneManager.h"

#include <imgui.h>

ClearScene::~ClearScene()
{
}

void ClearScene::LoadResources()
{
	// パイプライン
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline = std::make_unique<KGPlin>();
	spritePipeline->CreatePipelineAll(spriteShader, "Sprite");

	// タイトル名テクスチャ
	clearTex = TextureManager::Load("Resources/texture/kariResult.png");

	// プッシュAテクスチャ
	pushATex = TextureManager::Load("Resources/texture/kariNextScene.png");
}

void ClearScene::Init()
{
	input = KInput::GetInstance();

	camera = std::make_unique<DebugCamera>();

	sceneManager = SceneManager::GetInstance();

	// タイトル名
	clear = std::make_unique<Sprite>();
	clear->Init();
	clear->SetPipeline(spritePipeline.get());

	// プッシュA
	pushA = std::make_unique<Sprite>();
	pushA->Init();
	pushA->SetPipeline(spritePipeline.get());
}

void ClearScene::Update()
{
	camera->Update();

	// 次のシーンへ
	if (input->IsTrigger(DIK_SPACE) || input->GetPadButtonDown(XINPUT_GAMEPAD_A))
	{
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void ClearScene::ObjDraw()
{
}

void ClearScene::SpriteDraw()
{
	const float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	const float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	clear->Draw(clearTex, { width / 2, height * 1 / 3 }, { 1,1 });

	pushA->Draw(pushATex, { width / 2, height * 2 / 3 }, { 1,1 });
}

void ClearScene::Final()
{
}
