#include "RailCamera.h"
#include "MyMath.h"

void RailCamera::Init()
{
	cameraObject = std::make_unique<KObject3d>();
	cameraObject->Initialize();
	cameraObject->transform.pos = { 0,0,0 };
	cameraObject->transform.scale = { 1.0f,1.0f,1.0f };
	cameraObject->transform.rot = { 0,0,0 };

	viewProjection = std::make_unique<ViewProjection>();
	viewProjection->Initialize();
	viewProjection->aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();
}

void RailCamera::Update()
{
	cameraObject->transform.pos.z -= 0.5f;
	cameraObject->TransUpdate();

	viewProjection->eye = cameraObject->transform.pos;

	viewProjection->Update();
}

void RailCamera::SetPos(KMyMath::Vector3 pos)
{
}
