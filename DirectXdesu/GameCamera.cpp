#include "GameCamera.h"
#include "Player.h"

GameCamera::GameCamera()
{
	viewProjection.Initialize();
	viewProjection.aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();
}

void GameCamera::Update()
{
	Player* player = Player::nowPlayer;

	viewProjection.target = { 0,0,0 };

	viewProjection.eye.x = 0;
	viewProjection.eye.y = 50;
	viewProjection.eye.z = -200;

	viewProjection.Update();
}
