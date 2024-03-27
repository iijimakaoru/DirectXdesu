#pragma once
#pragma warning(push)
#pragma warning(disable: 4514)
#pragma warning(disable: 4668)
#include <DirectXMath.h>
#pragma warning(pop)
#include "MyMath.h"

/**
 * @file CollisionPrimitive.h
 * @brief 当たり判定の元
 * @author 飯島 薫
 */

struct Sphere
{
	// 中心座標
	KMyMath::Vector3 center_ = {0, 0, 0};
	// 半径
	float radius_ = 1.0f;
};

struct Plane
{
	// 法線ベクトル
	KMyMath::Vector3 normal = {0, 1, 0};
	// 原点(0,0,0)からの距離
	float distance = 0.0f;
};

class ColTriangle
{
public:
	// 頂点座標3つ
	KMyMath::Vector3 p0;
	KMyMath::Vector3 p1;
	KMyMath::Vector3 p2;
	// 法線ベクトル
	KMyMath::Vector3 normal;

	void ConputeNormal();
};

struct Ray
{
	// 視点座標
	KMyMath::Vector3 start = {0, 0, 0};
	// 方向
	KMyMath::Vector3 dir = {1, 0, 0};
};

class CollisionPrimitive
{
};

