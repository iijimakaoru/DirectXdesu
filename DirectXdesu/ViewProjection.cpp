#include "ViewProjection.h"
#include "KWindow.h"

ViewProjection::ViewProjection(int width, int height) {
	Initialize(width, height);
}

void ViewProjection::Initialize(int width, int height) {
	lenZ = -100;
	angleX = 0.0f;
	angleY = 0.0f;
	eye = { 0, 0, lenZ };
	target = { 0, 0, 0 };
	up = { 0, 1, 0 };

	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),						// �㉺��p45�x
		(float)width / height,							// �A�X�y�N�g��(��ʉ���/��ʏc��)
		0.1f, 1000.0f									// �O�[�A���[
	);
}

void ViewProjection::Update(int width, int height) {
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),						// �㉺��p45�x
		(float)width / height,							// �A�X�y�N�g��(��ʉ���/��ʏc��)
		0.1f, 1000.0f									// �O�[�A���[
	);
}