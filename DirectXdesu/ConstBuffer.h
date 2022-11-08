#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXTex.h>

using namespace DirectX;

struct ConstBufferData
{
	XMFLOAT4 color; // �F
	XMMATRIX mat; // 3D�ϊ��s��
};

// �萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
struct ConstBufferDataMaterial {
	XMFLOAT4 color; // �F
};

// �萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
struct ConstBufferDataTransform {
	XMMATRIX mat; // 3D�ϊ��s��
};