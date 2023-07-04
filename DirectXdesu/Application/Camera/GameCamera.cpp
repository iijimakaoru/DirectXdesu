#include "GameCamera.h"

void GameCamera::Init()
{
	viewProjection = std::make_unique<ViewProjection>();
	viewProjection->Initialize();
	viewProjection->aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();
}

void GameCamera::Update()
{

	viewProjection->target = { 0,0,0 };

	viewProjection->eye.x = 0;
	viewProjection->eye.y = 50;
	viewProjection->eye.z = -200;

	viewProjection->Update();
}
