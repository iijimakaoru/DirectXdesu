#include "Ease.h"

namespace MyMathUtility
{
	float Lerp(const float start, const float end, const float time)
	{
		return start + time * (end - start);
	}

	float EaseIn(const float start, const float end, const float time, const float power)
	{
		return Lerp(start, end, powf(time, power));
	}

	KMyMath::Vector3 EaseInVec3(const KMyMath::Vector3 start, const KMyMath::Vector3 end, const float time, const float power)
	{
		KMyMath::Vector3 result;

		result.x = EaseIn(start.x, end.x, time, power);
		result.y = EaseIn(start.y, end.y, time, power);
		result.z = EaseIn(start.z, end.z, time, power);
		return result;
	}

	float EaseOut(const float start, const float end, const float time, const float power)
	{
		return Lerp(start, end, 1 - powf(1 - time, power));
	}

	float EaseInSine(const float startPoint, const float endPoint, float nowFlame, float endFlame)
	{
		float len = startPoint - endPoint;

		return static_cast<float>(endPoint + len * cos(((nowFlame / endFlame) * PI) / 2));
	}

	float EaseOutSine(const float startPoint, const float endPoint, float nowFlame, float endFlame)
	{
		float len = endPoint - startPoint;

		return static_cast<float>(startPoint + len * sin(((nowFlame / endFlame) * PI) / 2));
	}
}