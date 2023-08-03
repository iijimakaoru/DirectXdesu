#include "ViewProjection.h"
#include "KWinApp.h"

ViewProjection::ViewProjection(){}

void ViewProjection::Initialize() 
{
	lenZ = -100;
	angleX = XMConvertToRadians(90.0f);
	angleY = 0.0f;
	eye = { angleX, angleY, lenZ };
	target = { 0, 0, 0 };
	up = { 0, 1, 0 };

	matView = MyMathUtility::MakeLockAt(eye, target, up);

	matProjection = MyMathUtility::MakePerspective(XMConvertToRadians(45.0f), aspect, 0.1f, 1000.0f);
}

void ViewProjection::Update() 
{
	matProjection = MyMathUtility::MakePerspective(XMConvertToRadians(45.0f), aspect, 0.1f, 1000.0f);
}