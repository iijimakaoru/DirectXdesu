#pragma once
#include "CollisionPrimitive.h"
#include "MyMath.h"

/**
 * @file Collision.h
 * @brief 当たり判定
 * @author 飯島 薫
 */
namespace MesiEngine {
class Collision {
public:
	// 球と平面の当たり判定
	static bool CheckSphere2Plane(
	    const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);
	static void ClosestPtPoint2Triangle(
	    const DirectX::XMVECTOR& point, const ColTriangle& triangle, DirectX::XMVECTOR* closet);
	// 球と三角形の当たり判定
	static bool CheckSphere2Triangle(
	    const Sphere& sphere, const ColTriangle& triangle, DirectX::XMVECTOR* inter = nullptr);
	// レイと平面の当たり判定
	static bool CheckRay2Plane(
	    const Ray& ray, const Plane& plane, float* distance = nullptr,
	    DirectX::XMVECTOR* inter = nullptr);
	// レイと三角形の当たり判定
	static bool CheckRay2Triangle(
	    const Ray& ray, const ColTriangle& triangle, float* distance = nullptr,
	    DirectX::XMVECTOR* inter = nullptr);
	// レイと球の当たり判定
	static bool CheckRay2Sphere(
	    const Ray& ray, const Sphere& sphere, float* distance = nullptr,
	    DirectX::XMVECTOR* inter = nullptr);
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
} // namespace MyCollisions
} // namespace MesiEngine