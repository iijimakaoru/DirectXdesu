#include "KMyGame.h"

void KMyGame::Init()
{
	Framework::Init();

	gameScene = std::make_unique<GameScence>();

	imguiMane.Init();
}

void KMyGame::Update()
{
	Framework::Update();

	imguiMane.Begin();

	gameScene->Update();

	imguiMane.End();
}

void KMyGame::Draw()
{
	// •`‰æŠJŽn
	dx->PreDraw();

	gameScene->Draw();

	imguiMane.Draw();

	dx->PostDraw();
}

void KMyGame::Final()
{
	Framework::Final();

	gameScene.release();
}
