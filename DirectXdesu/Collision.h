#pragma once
#include "CollisionPrimitive.h"

class Collision
{
public:
	// ���ƕ��ʂ̓����蔻��
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closet);
	// ���ƎO�p�`�̓����蔻��
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr);
	// ���C�ƕ��ʂ̓����蔻��
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	// ���C�ƎO�p�`�̓����蔻��
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	// ���C�Ƌ��̓����蔻��
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
};

