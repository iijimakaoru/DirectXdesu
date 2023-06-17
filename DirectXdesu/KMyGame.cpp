#include "KMyGame.h"
#include "GameScence.h"
#include "TitleScene.h"

void KMyGame::Init()
{
	Framework::Init();

	scene = std::make_unique<GameScence>();

	imguiMane.Init();
}

void KMyGame::Update()
{
	Framework::Update();

	imguiMane.Begin();

	scene->Update();

	imguiMane.End();
}

void KMyGame::Draw()
{
	// •`‰æŠJŽn
	dx->PreDraw();

	scene->Draw();

	imguiMane.Draw();

	dx->PostDraw();
}

void KMyGame::Final()
{
	Framework::Final();

	scene.release();
}
