#include "Framework.h"
#include "Sprite.h"
#include "BaseScene.h"
#include "ParticleManager.h"

void Framework::Init()
{
	// FBX初期化
	fbxLoader = FbxLoader::GetInstance();
	fbxLoader->Init();

	// ウィンドウ初期化
	win = KWinApp::GetInstance();
	win->Init();

	// DirectX初期化
	dx = KDirectXCommon::GetInstance();
	dx->Init();

	// Input初期化
	input = KInput::GetInstance();
	input->Init();

	sound = Sound::GetInstance();
	sound->Init();

	seManager = SEManager::GetInstance();
	seManager->Init();
	seManager->LoadSE();

	bgmManager = BGMManager::GetInstance();
	bgmManager->Init();
	bgmManager->LoadBGM();

	// 静的初期化
	Sprite::StaticInit();
	PostEffect::StaticInit();

	// シーンマネージャー生成
	sceneManager = SceneManager::GetInstance();

	// テクスチャマネージャー初期化
	textureManager = TextureManager::GetInstance();
	textureManager->Init();

	Particles::StaticInitialize();

	// Imgui初期化
	imguiMane.Init();

	// Spriteパイプライン
	spriteShader.Init(L"Resources/Shader/PostEffectVS.hlsl", L"Resources/Shader/PostEffectPS.hlsl");
	spritePipeline = std::make_unique<KGPlin>();
	spritePipeline->CreatePipelineAll(spriteShader, "PostEffect");

	// ポストエフェクトテスト
	postEffect = std::make_unique<PostEffect>();
	postEffect->Init();
	postEffect->SetPipeline(spritePipeline.get());

	sceneChange = SceneChange::GetInstance();
	sceneChange->Init();
}

void Framework::Final()
{
	Sound::GetInstance()->GetxAudio().Reset();

	// シーンマネージャーの解放
	sceneManager->Final();

	// テクスチャマネージャーの解放
	textureManager->Delete();

	// パーティクルマネージャー解放
	ParticleManager::GetInstance()->Delete();
	ObjParticleManager::GetInstance()->Delete();

	seManager->Finalize();

	bgmManager->Finalize();

	// シーンファクトリー解放
	delete sceneFactory;

	sceneChange->Delete();

	dx->Destroy();

	fbxLoader->Finalize();
}

void Framework::Update()
{
	// 更新
	input->Update();

	// Imgui更新はじめ
	imguiMane.Begin();

	// シーンマネージャーの更新
	sceneManager->Update();

	// Imgui更新終了
	imguiMane.End();
}

bool Framework::IsEndRwquest()
{
	if (win->ProcessMessage() || input->IsTrigger(DIK_ESCAPE))
	{
		return true;
	}

	return false;
}

void Framework::Run()
{
	Init();

	while (true)
	{
		// 終了リクエスト
		if (IsEndRwquest())
		{
			break;
		}

		// 更新
		Update();
		// 描画
		Draw();
	}
	// 終了
	Final();
}
