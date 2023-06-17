#include "KMyGame.h"
#include "GameScence.h"
#include "TitleScene.h"

void KMyGame::Init()
{
	Framework::Init();

	// 最初のシーン作成
	BaseScene* scene = new TitleScene();
	// シーンマネージャーにシーンセット
	sceneManager->SetNestScene(scene);
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
