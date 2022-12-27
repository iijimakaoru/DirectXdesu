#include "ViewProjection.h"
#include "KWinApp.h"

ViewProjection::ViewProjection(){}

void ViewProjection::Initialize(int width, int height) {
	lenZ = -100;
	angleX = 0.0f;
	angleY = 0.0f;
	eye = { angleX, angleY, lenZ };
	target = { 0, 0, 0 };
	up = { 0, 1, 0 };

	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),						// 上下画角45度
		(float)width / height,							// アスペクト比(画面横幅/画面縦幅)
		0.1f, 1000.0f									// 前端、奥端
	);
}

void ViewProjection::Update(int width, int height) {
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}