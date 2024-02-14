#include "Framework.h"
#include "BaseScene.h"
#include "ParticleManager.h"
#include "Sprite.h"
#include "PipelineManager.h"
#include "ResourceManager.h"

void Framework::Init() {
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

	// 
	audioManager = AudioManager::GetInstance();
	audioManager->Init();

	// パイプライン生成
	PipelineManager::GetInstance()->Init();

	// 静的初期化
	Sprite::StaticInit();
	PostEffect::StaticInit();

	// シーンマネージャー生成
	sceneManager = SceneManager::GetInstance();

	// テクスチャマネージャー初期化
	textureManager = TextureManager::GetInstance();
	textureManager->Init();
	textureManager->LoadTextures();

	// リソース読み込み
	ResourceManager::GetInstance()->Init();


	Particles::StaticInitialize();

	// Imgui初期化
	imguiMane.Init();

	// ポストエフェクトテスト
	postEffect = std::make_unique<PostEffect>();
	postEffect->Init();
	postEffect->SetPipeline(PipelineManager::GetInstance()->GetPostPipeline());

	sceneChange = SceneChange::GetInstance();
	sceneChange->Init();
}

void Framework::Final() {
	// シーンマネージャーの解放
	sceneManager->Final();

	// テクスチャマネージャーの解放
	textureManager->Delete();

	// パーティクルマネージャー解放
	ParticleManager::GetInstance()->Delete();
	ObjParticleManager::GetInstance()->Delete();

	audioManager->Finalize();

	// シーンファクトリー解放
	delete sceneFactory;

	sceneChange->Delete();

	dx->Destroy();

	fbxLoader->Finalize();
}

void Framework::Update() {
	// 更新
	input->Update();

	// Imgui更新はじめ
	imguiMane.Begin();

	// シーンマネージャーの更新
	sceneManager->Update();

	// Imgui更新終了
	imguiMane.End();
}

bool Framework::IsEndRwquest() {
	if (win->ProcessMessage() || input->IsTrigger(DIK_ESCAPE)) {
		return true;
	}

	return false;
}

void Framework::Run() {
	Init();

	while (true) {
		// 終了リクエスト
		if (IsEndRwquest()) {
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
