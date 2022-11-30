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

struct ConstBufferDataB0
{
	XMMATRIX mat;
};

struct ConstBufferDataB1
{
	XMFLOAT3 ambient;
	float pad1;
	XMFLOAT3 diffuse;
	float pad2;
	XMFLOAT3 specular;
	float alpha;
};