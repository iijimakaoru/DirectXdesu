#include "KMyGame.h"
#include "SceneFactory.h"

void KMyGame::Init()
{
	Framework::Init();

	// シーンファクトリーを生成
	sceneFactory = new SceneFactory();
	// シーンマネージャーにセット
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);
	// 最初のシーン
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

void KMyGame::Update()
{
	Framework::Update();
}

void KMyGame::Draw()
{
	// レンダーテクスチャへの描画
	postEffect->PreDrawScene();

	// シーンマネージャーの描画
	sceneManager->Draw();

	postEffect->PostDrawScene();

	// 描画開始
	dx->PreDraw();

	// ポストエフェクト描画
	postEffect->Draw();

	// Imgui描画
	imguiMane.Draw();

	dx->PostDraw();
}

void KMyGame::Final()
{
	Framework::Final();
}
