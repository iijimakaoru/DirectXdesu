#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXTex.h>

using namespace DirectX;

struct ConstBufferData
{
	XMFLOAT4 color; // F
	XMMATRIX mat; // 3D•ÏŠ·s—ñ
};