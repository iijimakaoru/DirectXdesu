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

	//// タイトル名テクスチャ
	//titleTex.CreateTexture("Resources/texture/", "kariTitle.png");
}

void TitleScene::Init()
{
	input = KInput::GetInstance();

	camera = std::make_unique<DebugCamera>();

	sceneManager = SceneManager::GetInstance();

	//// タイトル名
	//titleName = std::make_unique<Sprite>();
	//titleName->Init();
	//titleName->SetPipeline(spritePipeline.get());
}

void TitleScene::Update()
{
	camera->Update();

	// 次のシーンへ
	if (input->IsTrigger(DIK_SPACE))
	{
		// シーン切り替え依頼
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
