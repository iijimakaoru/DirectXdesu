#include "KMyGame.h"

void KMyGame::Init()
{
	Framework::Init();
}

void KMyGame::Update()
{
	Framework::Update();
}

void KMyGame::Draw()
{
	// �`��J�n
	dx->PreDraw();

	gameScene->Draw();

	imguiMane.Draw();

	dx->PostDraw();
}

void KMyGame::Final()
{
	Framework::Final();
}
