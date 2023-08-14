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
	// パイプライン
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline = std::make_unique<KGPlin>();
	spritePipeline->CreatePipelineAll(spriteShader, "Sprite");

	// タイトル名テクスチャ
	titleTex = TextureManager::Load("Resources/texture/kariTitle.png");

	// プッシュAテクスチャ
	pushATex = TextureManager::Load("Resources/texture/kariNextScene.png");
}

void TitleScene::Init()
{
	input = KInput::GetInstance();

	camera = std::make_unique<DebugCamera>();

	sceneManager = SceneManager::GetInstance();

	// タイトル名
	titleName = std::make_unique<Sprite>();
	titleName->Init();
	titleName->SetPipeline(spritePipeline.get());

	// プッシュA
	pushA = std::make_unique<Sprite>();
	pushA->Init();
	pushA->SetPipeline(spritePipeline.get());
}

void TitleScene::Update()
{
	camera->Update();

	// 次のシーンへ
	if (input->IsTrigger(DIK_SPACE) || input->GetPadButtonDown(XINPUT_GAMEPAD_A))
	{
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void TitleScene::ObjDraw()
{
	
}

void TitleScene::SpriteDraw()
{
	const float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	const float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	titleName->Draw(titleTex, { width / 2, height * 1 / 3 }, { 1,1 });

	pushA->Draw(pushATex, { width / 2, height * 2 / 3 }, { 1,1 });
}

void TitleScene::Final()
{
	
}
