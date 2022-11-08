#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXTex.h>

using namespace DirectX;

struct ConstBufferData
{
	XMFLOAT4 color; // 色
	XMMATRIX mat; // 3D変換行列
};

// 定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial {
	XMFLOAT4 color; // 色
};

// 定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform {
	XMMATRIX mat; // 3D変換行列
};