#include "Ease.h"

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