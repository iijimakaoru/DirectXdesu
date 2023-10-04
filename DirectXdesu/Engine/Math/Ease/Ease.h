#pragma
#include "MyMath.h"

namespace MyEase
{
	// ï‚ä‘
	const float Lerp(const float start, const float end, const float time);
	const KMyMath::Vector2 Lerp2D(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time);
	const KMyMath::Vector3 Lerp3D(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time);
	
	// ìÒèÊ
	float InQuadFloat(const float start, const float end, const float time);
	float OutQuadFloat(const float start, const float end, const float time);
	float InOutQuadFloat(const float start, const float end, const float time);

	// 2D
	KMyMath::Vector2 InQuadVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time);
	KMyMath::Vector2 OutQuadVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time);
	KMyMath::Vector2 InOutQuadVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time);

	// 3D
	KMyMath::Vector3 InQuadVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time);
	KMyMath::Vector3 OutQuadVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time);
	KMyMath::Vector3 InOutQuadVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time);

	// éOèÊ
	float InCubicFloat(const float start, const float end, const float time);
	float OutCubicFloat(const float start, const float end, const float time);
	float InOutCubicFloat(const float start, const float end, const float time);

	// 2D
	KMyMath::Vector2 InCubicVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time);
	KMyMath::Vector2 OutCubicVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time);
	KMyMath::Vector2 InOutCubicVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time);

	// 3D
	KMyMath::Vector3 InCubicVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time);
	KMyMath::Vector3 OutCubicVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time);
	KMyMath::Vector3 InOutCubicVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time);

	// élèÊ
	float InQuartFloat(const float start, const float end, const float time);
	float OutQuartFloat(const float start, const float end, const float time);
	float InOutQuartFloat(const float start, const float end, const float time);

	// 2D
	KMyMath::Vector2 InQuartVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time);
	KMyMath::Vector2 OutQuartVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time);
	KMyMath::Vector2 InOutQuartVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time);

	// 3D
	KMyMath::Vector3 InQuartVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time);
	KMyMath::Vector3 OutQuartVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time);
	KMyMath::Vector3 InOutQuartVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time);

	// å‹èÊ
	float InQuintFloat(const float start, const float end, const float time);
	float OutQuintFloat(const float start, const float end, const float time);
	float InOutQuintFloat(const float start, const float end, const float time);

	// 2D
	KMyMath::Vector2 InQuintVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time);
	KMyMath::Vector2 OutQuintVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time);
	KMyMath::Vector2 InOutQuintVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time);

	// 3D
	KMyMath::Vector3 InQuintVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time);
	KMyMath::Vector3 OutQuintVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time);
	KMyMath::Vector3 InOutQuintVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time);
}

