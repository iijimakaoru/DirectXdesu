#pragma once
#include "CollisionPrimitive.h"
#include "MyMath.h"

/**
 * @file Collision.h
 * @brief 当たり判定
 * @author 飯島 薫
 */

class Collision {
public:
	// 球と平面の当たり判定
	static bool CheckSphere2Plane(
	    const Sphere& sphere, const Plane& plane, KMyMath::Vector3* inter = nullptr);
	static void ClosestPtPoint2Triangle(
	    const KMyMath::Vector3& point, const ColTriangle& triangle, KMyMath::Vector3* closet);
	// 球と三角形の当たり判定
	static bool CheckSphere2Triangle(
	    const Sphere& sphere, const ColTriangle& triangle, KMyMath::Vector3* inter = nullptr);
	// レイと平面の当たり判定
	static bool CheckRay2Plane(
	    const Ray& ray, const Plane& plane, float* distance = nullptr,
	    KMyMath::Vector3* inter = nullptr);
	// レイと三角形の当たり判定
	static bool CheckRay2Triangle(
	    const Ray& ray, const ColTriangle& triangle, float* distance = nullptr,
	    KMyMath::Vector3* inter = nullptr);
	// レイと球の当たり判定
	static bool CheckRay2Sphere(
	    const Ray& ray, const Sphere& sphere, float* distance = nullptr,
	    KMyMath::Vector3* inter = nullptr);
};

namespace MyCollisions {
/// <summary>
/// 球と球
/// </summary>
/// <param name="posA"></param>
/// <param name="posB"></param>
/// <param name="radiusA"></param>
/// <param name="radiusB"></param>
/// <returns></returns>
bool CheckSphereToSphere(
    const KMyMath::Vector3& posA, const KMyMath::Vector3& posB, const float radiusA,
    const float radiusB);

bool CheckBoxToBox(
    const KMyMath::Vector3& boxPosA, const KMyMath::Vector3& boxPosB,
    const KMyMath::Vector3& boxScaleA, const KMyMath::Vector3& boxScaleB);
} // namespace MyCollisions
