#pragma once
#include <DirectXMath.h>
#include <DirectXTex.h>
#include "Vector3.h"

using namespace DirectX;

class ViewProjection
{
public:
	ViewProjection(int width, int height);
	void Initialize(int width, int height);
	void Update(int width, int height);

	// �J�����̋���
	float lenZ;
	float angleX; // �J�����̉�]�pX
	float angleY; // �J�����̉�]�pY
	// �r���[�ϊ��s��
	XMMATRIX matView;
	XMFLOAT3 eye;
	XMFLOAT3 target;
	XMFLOAT3 up;
	// �ˉe�ϊ��s��̌v�Z
	XMMATRIX matProjection;
};

