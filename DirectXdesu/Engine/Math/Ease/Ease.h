#pragma 
#include <math.h>
#include "Vector3.h"
#include "MyMath.h"

namespace MyEase
{
	float Lerp(const float start, const float end, const float time);
	float EaseIn(const float start, const float end, const float time, const float power);
	KMyMath::Vector3 EaseInVec3(const KMyMath::Vector3 start, const KMyMath::Vector3 end, const float time, const float power);
	float EaseOut(const float start, const float end, const float time, const float power);

	/// <summary>
	/// イーズイン
	/// </summary>
	/// <param name="startPoint"></param>
	/// <param name="endPoint"></param>
	/// <param name="nowFlame"></param>
	/// <param name="endFlame"></param>
	/// <returns></returns>
	float EaseInSine(const float startPoint, const float endPoint, float nowFlame, float endFlame);

	/// <summary>
	/// イーズアウト
	/// </summary>
	/// <param name="startPoint"></param>
	/// <param name="endPoint"></param>
	/// <param name="nowFlame"></param>
	/// <param name="endFlame"></param>
	/// <returns></returns>
	float EaseOutSine(const float startPoint, const float endPoint, float nowFlame, float endFlame);

	/// <summary>
	/// イーズイン(3D)
	/// </summary>
	/// <param name="startPoint"></param>
	/// <param name="endPoint"></param>
	/// <param name="nowFlame"></param>
	/// <param name="endFlame"></param>
	/// <returns></returns>
	KMyMath::Vector3 EaseInSine3D(const KMyMath::Vector3 startPoint, const KMyMath::Vector3 endPoint, float nowFlame, float endFlame);

	/// <summary>
	/// イーズアウト(3D)
	/// </summary>
	/// <param name="startPoint"></param>
	/// <param name="endPoint"></param>
	/// <param name="nowFlame"></param>
	/// <param name="endFlame"></param>
	/// <returns></returns>
	KMyMath::Vector3 EaseOutSine3D(const KMyMath::Vector3 startPoint, const KMyMath::Vector3 endPoint, float nowFlame, float endFlame);

	// 二乗
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

	// 三乗
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

	// 四乗
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

	// 五乗
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

