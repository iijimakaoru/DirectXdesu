#pragma 
#include <math.h>
#include "Vector3.h"
#include "MyMath.h"

namespace MyMathUtility
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
}

