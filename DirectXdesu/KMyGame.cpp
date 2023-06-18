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
	// 描画開始
	dx->PreDraw();

	// シーンマネージャーの描画
	sceneManager->Draw();

	// Imgui描画
	imguiMane.Draw();

	dx->PostDraw();
}

void KMyGame::Final()
{
	Framework::Final();
}
