#include "KMyGame.h"
#include "SceneFactory.h"
#include<ModelManager.h>

void KMyGame::Init() {
	Framework::Init();

	// シーンファクトリーを生成
	sceneFactory = new SceneFactory();
	// シーンマネージャーにセット
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);

	ModelManager::Load("skydome", "S_SkyDorm",true);
	ModelManager::Load("cube","S_Cube");
	ModelManager::Load("arrow","S_Arrow");

	// 最初のシーン
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

void KMyGame::Update() {
	Framework::Update();
	postEffectManager->Update();
}

void KMyGame::Draw() {
	// レンダーテクスチャへの描画
	KDirectXCommon::GetInstance()->BeginCommnd();
	postEffectManager->PreDraw(sceneManager);

	// 描画開始
	dx->PreDraw();

	// ポストエフェクト描画
	// postEffect->Draw();
	postEffectManager->Draw();

	// Imgui描画
#ifdef _DEBUG
	imguiMane.Draw();
#endif

	dx->PostDraw();
}

void KMyGame::Final() {
	Framework::Final();
}
