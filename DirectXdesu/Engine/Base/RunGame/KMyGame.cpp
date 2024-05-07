#include "KMyGame.h"
#include "BulletManager.h"
#include "SceneFactory.h"

void KMyGame::Init() {
	Framework::Init();

	// シーンファクトリーを生成
	sceneFactory = new SceneFactory();
	// シーンマネージャーにセット
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);

	// 最初のシーン
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

void KMyGame::Update() {
	Framework::Update();
	postEffectManager->Update();
}

void KMyGame::Draw() {
	// レンダーテクスチャへの描画
	postEffectManager->PreDraw(sceneManager);

	// シーンマネージャーの描画
	// sceneManager->Draw();

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

	BulletManager::GetInstance()->Delete();
}
