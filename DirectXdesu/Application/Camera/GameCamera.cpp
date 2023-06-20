#include "GameCamera.h"

GameCamera::GameCamera()
{
	viewProjection.Initialize();
	viewProjection.aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();
}

void GameCamera::Update()
{

	viewProjection.target = { 0,0,0 };

	viewProjection.eye.x = 0;
	viewProjection.eye.y = 50;
	viewProjection.eye.z = -200;

	viewProjection.Update();
}
