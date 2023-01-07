#include "ViewProjection.h"
#include "KWinApp.h"

ViewProjection::ViewProjection(){}

void ViewProjection::Initialize() {
	lenZ = -100;
	angleX = XMConvertToRadians(90.0f);
	angleY = 0.0f;
	eye = { angleX, angleY, lenZ };
	target = { 0, 0, 0 };
	up = { 0, 1, 0 };

	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),						// 上下画角45度
		aspect,// アスペクト比(画面横幅/画面縦幅)
		0.1f, 1000.0f									// 前端、奥端
	);
}

void ViewProjection::Update() {
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),						// 上下画角45度
		aspect,// アスペクト比(画面横幅/画面縦幅)
		0.1f, 1000.0f									// 前端、奥端
	);
}