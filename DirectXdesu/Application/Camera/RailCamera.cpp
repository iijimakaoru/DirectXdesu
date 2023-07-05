#include "RailCamera.h"
#include "MyMath.h"

void RailCamera::Init()
{
	object = std::make_unique<KObject3d>();
	object->Initialize();

	viewProjection = std::make_unique<ViewProjection>();
	viewProjection->Initialize();
	viewProjection->aspect = (float)KWinApp::GetWindowSizeW() / KWinApp::GetWindowSizeH();
}

void RailCamera::Update()
{
	object->transform.pos.z += 1;
	object->TransUpdate();
}

void RailCamera::SetPos(KMyMath::Vector3 pos)
{
}
