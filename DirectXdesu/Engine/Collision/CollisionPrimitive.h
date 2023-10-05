#pragma once
#pragma warning(push)
#pragma warning(disable: 4514)
#pragma warning(disable: 4668)
#include <DirectXMath.h>
#pragma warning(pop)

struct Sphere
{
	// 中心座標
	DirectX::XMVECTOR center = { 0,0,0,1 };
	// 半径
	float radius = 1.0f;
};

struct Plane
{
	// 法線ベクトル
	DirectX::XMVECTOR normal = { 0,1,0,0 };
	// 原点(0,0,0)からの距離
	float distance = 0.0f;
};

class ColTriangle
{
public:
	// 頂点座標3つ
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	// 法線ベクトル
	DirectX::XMVECTOR normal;

	void ConputeNormal();
};

struct Ray
{
	// 視点座標
	DirectX::XMVECTOR start = { 0,0,0,1 };
	// 方向
	DirectX::XMVECTOR dir = { 1,0,0,0 };
};

class CollisionPrimitive
{
};

