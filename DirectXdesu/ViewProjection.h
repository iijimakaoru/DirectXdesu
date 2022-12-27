#pragma once
#include <DirectXMath.h>
#include <DirectXTex.h>
#include "Vector3.h"

using namespace DirectX;

class ViewProjection
{
public:
	ViewProjection();
	void Initialize(int width, int height);
	void Update(int width, int height);

	// �J�����̋���
	float lenZ = 0;
	float angleX = 0; // �J�����̉�]�pX
	float angleY = 0; // �J�����̉�]�pY
	// �r���[�ϊ��s��
	XMMATRIX matView{};
	XMFLOAT3 eye{};
	XMFLOAT3 target{};
	XMFLOAT3 up{};
	// �ˉe�ϊ��s��̌v�Z
	XMMATRIX matProjection{};
};

