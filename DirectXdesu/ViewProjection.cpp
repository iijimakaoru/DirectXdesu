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
		XMConvertToRadians(45.0f),						// �㉺��p45�x
		aspect,// �A�X�y�N�g��(��ʉ���/��ʏc��)
		0.1f, 1000.0f									// �O�[�A���[
	);
}

void ViewProjection::Update() {
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),						// �㉺��p45�x
		aspect,// �A�X�y�N�g��(��ʉ���/��ʏc��)
		0.1f, 1000.0f									// �O�[�A���[
	);
}