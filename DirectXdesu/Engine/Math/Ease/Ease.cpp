#include "Ease.h"

namespace MyEase
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

		return static_cast<float>(endPoint + len * cos(((nowFlame / endFlame) * MyMathUtility::PI) / 2));
	}

	float EaseOutSine(const float startPoint, const float endPoint, float nowFlame, float endFlame)
	{
		float len = endPoint - startPoint;

		return static_cast<float>(startPoint + len * sin(((nowFlame / endFlame) * MyMathUtility::PI) / 2));
	}

	KMyMath::Vector3 EaseInSine3D(const KMyMath::Vector3 startPoint, const KMyMath::Vector3 endPoint, float nowFlame, float endFlame)
	{
		KMyMath::Vector3 result;

		result.x = EaseInSine(startPoint.x, endPoint.x, nowFlame, endFlame);
		result.y = EaseInSine(startPoint.y, endPoint.y, nowFlame, endFlame);
		result.z = EaseInSine(startPoint.z, endPoint.z, nowFlame, endFlame);

		return result;
	}

	KMyMath::Vector3 EaseOutSine3D(const KMyMath::Vector3 startPoint, const KMyMath::Vector3 endPoint, float nowFlame, float endFlame)
	{
		KMyMath::Vector3 result;

		result.x = EaseOutSine(startPoint.x, endPoint.x, nowFlame, endFlame);
		result.y = EaseOutSine(startPoint.y, endPoint.y, nowFlame, endFlame);
		result.z = EaseOutSine(startPoint.z, endPoint.z, nowFlame, endFlame);

		return result;
	}

	float InQuadFloat(const float start, const float end, const float time)
	{
		float t = powf(time, 2);

		return start * (1.0f - t) + end * t;
	}

	float OutQuadFloat(const float start, const float end, const float time)
	{
		float t = (1 - powf(1 - time, 2));

		return start * (1.0f - t) + end * t;
	}

	float InOutQuadFloat(const float start, const float end, const float time)
	{
		float t;
		if (time < 0.5f)
		{
			t = 2 * powf(time, 2);
		}
		else
		{
			t = (1 - powf(-2 * time + 2, 2) / 2);
		}

		return start * (1.0f - t) + end * t;
	}

	KMyMath::Vector2 InQuadVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time)
	{
		float t = powf(time, 2);
		KMyMath::Vector2 A, B;
		A = KMyMath::Vector2(start.x * (1.0f - t), start.y * (1.0f - t));
		B = KMyMath::Vector2(end.x * t, end.y * t);

		KMyMath::Vector2 position;
		position = KMyMath::Vector2(A.x + B.x, A.y + B.y);
		return position;
	}

	KMyMath::Vector2 OutQuadVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time)
	{
		float t = (1 - powf(1 - time, 2));
		KMyMath::Vector2 A, B;
		A = KMyMath::Vector2(start.x * (1.0f - t), start.y * (1.0f - t));
		B = KMyMath::Vector2(end.x * t, end.y * t);

		KMyMath::Vector2 position;
		position = KMyMath::Vector2(A.x + B.x, A.y + B.y);
		return position;
	}

	KMyMath::Vector2 InOutQuadVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time)
	{
		float t;
		if (time < 0.5f)
		{
			t = 2 * powf(time, 2);
		}
		else
		{
			t = (1 - powf(-2 * time + 2, 2) / 2);
		}

		KMyMath::Vector2 A, B;
		A = KMyMath::Vector2(start.x * (1.0f - t), start.y * (1.0f - t));
		B = KMyMath::Vector2(end.x * t, end.y * t);

		KMyMath::Vector2 position;
		position = KMyMath::Vector2(A.x + B.x, A.y + B.y);
		return position;
	}

	KMyMath::Vector3 InQuadVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time)
	{
		float t = powf(time, 2);
		KMyMath::Vector3 A, B;
		A = KMyMath::Vector3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
		B = KMyMath::Vector3(end.x * t, end.y * t, end.z * t);

		KMyMath::Vector3 position;
		position = KMyMath::Vector3(A.x + B.x, A.y + B.y, A.z + B.z);
		return position;
	}

	KMyMath::Vector3 OutQuadVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time)
	{
		float t = (1 - powf(1 - time, 2));
		KMyMath::Vector3 A, B;
		A = KMyMath::Vector3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
		B = KMyMath::Vector3(end.x * t, end.y * t, end.z * t);

		KMyMath::Vector3 position;
		position = KMyMath::Vector3(A.x + B.x, A.y + B.y, A.z + B.z);
		return position;
	}

	KMyMath::Vector3 InOutQuadVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time)
	{
		float t;
		if (time < 0.5f)
		{
			t = 2 * powf(time, 2);
		}
		else
		{
			t = (1 - powf(-2 * time + 2, 2) / 2);
		}

		KMyMath::Vector3 A, B;
		A = KMyMath::Vector3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
		B = KMyMath::Vector3(end.x * t, end.y * t, end.z * t);

		KMyMath::Vector3 position;
		position = KMyMath::Vector3(A.x + B.x, A.y + B.y, A.z + B.z);
		return position;
	}

	float InCubicFloat(const float start, const float end, const float time)
	{
		float t = powf(time, 3);

		return start * (1.0f - t) + end * t;
	}

	float OutCubicFloat(const float start, const float end, const float time)
	{
		float t = (1 - powf(1 - time, 3));

		return start * (1.0f - t) + end * t;
	}

	float InOutCubicFloat(const float start, const float end, const float time)
	{
		float t;
		if (time < 0.5f)
		{
			t = 4 * powf(time, 3);
		}
		else
		{
			t = (1 - powf(-2 * time + 2, 3) / 2);
		}

		return start * (1.0f - t) + end * t;
	}

	KMyMath::Vector2 InCubicVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time)
	{
		float t = powf(time, 3);
		KMyMath::Vector2 A, B;
		A = KMyMath::Vector2(start.x * (1.0f - t), start.y * (1.0f - t));
		B = KMyMath::Vector2(end.x * t, end.y * t);

		KMyMath::Vector2 position;
		position = KMyMath::Vector2(A.x + B.x, A.y + B.y);
		return position;
	}

	KMyMath::Vector2 OutCubicVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time)
	{
		float t = (1 - powf(1 - time, 3));
		KMyMath::Vector2 A, B;
		A = KMyMath::Vector2(start.x * (1.0f - t), start.y * (1.0f - t));
		B = KMyMath::Vector2(end.x * t, end.y * t);

		KMyMath::Vector2 position;
		position = KMyMath::Vector2(A.x + B.x, A.y + B.y);
		return position;
	}

	KMyMath::Vector2 InOutCubicVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time)
	{
		float t;
		if (time < 0.5f)
		{
			t = 4 * powf(time, 3);
		}
		else
		{
			t = (1 - powf(-2 * time + 2, 3) / 2);
		}

		KMyMath::Vector2 A, B;
		A = KMyMath::Vector2(start.x * (1.0f - t), start.y * (1.0f - t));
		B = KMyMath::Vector2(end.x * t, end.y * t);

		KMyMath::Vector2 position;
		position = KMyMath::Vector2(A.x + B.x, A.y + B.y);
		return position;
	}

	KMyMath::Vector3 InCubicVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time)
	{
		float t = powf(time, 3);
		KMyMath::Vector3 A, B;
		A = KMyMath::Vector3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
		B = KMyMath::Vector3(end.x * t, end.y * t, end.z * t);

		KMyMath::Vector3 position;
		position = KMyMath::Vector3(A.x + B.x, A.y + B.y, A.z + B.z);
		return position;
	}

	KMyMath::Vector3 OutCubicVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time)
	{
		float t = (1 - powf(1 - time, 3));
		KMyMath::Vector3 A, B;
		A = KMyMath::Vector3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
		B = KMyMath::Vector3(end.x * t, end.y * t, end.z * t);

		KMyMath::Vector3 position;
		position = KMyMath::Vector3(A.x + B.x, A.y + B.y, A.z + B.z);
		return position;
	}

	KMyMath::Vector3 InOutCubicVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time)
	{
		float t;
		if (time < 0.5f)
		{
			t = 4 * powf(time, 3);
		}
		else
		{
			t = (1 - powf(-2 * time + 2, 3) / 2);
		}

		KMyMath::Vector3 A, B;
		A = KMyMath::Vector3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
		B = KMyMath::Vector3(end.x * t, end.y * t, end.z * t);

		KMyMath::Vector3 position;
		position = KMyMath::Vector3(A.x + B.x, A.y + B.y, A.z + B.z);
		return position;
	}

	float InQuartFloat(const float start, const float end, const float time)
	{
		float t = powf(time, 4);

		return start * (1.0f - t) + end * t;
	}

	float OutQuartFloat(const float start, const float end, const float time)
	{
		float t = (1 - powf(1 - time, 4));

		return start * (1.0f - t) + end * t;
	}

	float InOutQuartFloat(const float start, const float end, const float time)
	{
		float t;
		if (time < 0.5f)
		{
			t = 8 * powf(time, 4);
		}
		else
		{
			t = (1 - powf(-2 * time + 2, 4) / 2);
		}

		return start * (1.0f - t) + end * t;
	}

	KMyMath::Vector2 InQuartVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time)
	{
		float t = powf(time, 4);
		KMyMath::Vector2 A, B;
		A = KMyMath::Vector2(start.x * (1.0f - t), start.y * (1.0f - t));
		B = KMyMath::Vector2(end.x * t, end.y * t);

		KMyMath::Vector2 position;
		position = KMyMath::Vector2(A.x + B.x, A.y + B.y);
		return position;
	}

	KMyMath::Vector2 OutQuartVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time)
	{
		float t = (1 - powf(1 - time, 4));
		KMyMath::Vector2 A, B;
		A = KMyMath::Vector2(start.x * (1.0f - t), start.y * (1.0f - t));
		B = KMyMath::Vector2(end.x * t, end.y * t);

		KMyMath::Vector2 position;
		position = KMyMath::Vector2(A.x + B.x, A.y + B.y);
		return position;
	}

	KMyMath::Vector2 InOutQuartVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time)
	{
		float t;
		if (time < 0.5f)
		{
			t = 8 * powf(time, 4);
		}
		else
		{
			t = (1 - powf(-2 * time + 2, 4) / 2);
		}

		KMyMath::Vector2 A, B;
		A = KMyMath::Vector2(start.x * (1.0f - t), start.y * (1.0f - t));
		B = KMyMath::Vector2(end.x * t, end.y * t);

		KMyMath::Vector2 position;
		position = KMyMath::Vector2(A.x + B.x, A.y + B.y);
		return position;
	}

	KMyMath::Vector3 InQuartVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time)
	{
		float t = powf(time, 4);
		KMyMath::Vector3 A, B;
		A = KMyMath::Vector3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
		B = KMyMath::Vector3(end.x * t, end.y * t, end.z * t);

		KMyMath::Vector3 position;
		position = KMyMath::Vector3(A.x + B.x, A.y + B.y, A.z + B.z);
		return position;
	}

	KMyMath::Vector3 OutQuartVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time)
	{
		float t = (1 - powf(1 - time, 4));
		KMyMath::Vector3 A, B;
		A = KMyMath::Vector3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
		B = KMyMath::Vector3(end.x * t, end.y * t, end.z * t);

		KMyMath::Vector3 position;
		position = KMyMath::Vector3(A.x + B.x, A.y + B.y, A.z + B.z);
		return position;
	}

	KMyMath::Vector3 InOutQuartVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time)
	{
		float t;
		if (time < 0.5f)
		{
			t = 8 * powf(time, 4);
		}
		else
		{
			t = (1 - powf(-2 * time + 2, 4) / 2);
		}

		KMyMath::Vector3 A, B;
		A = KMyMath::Vector3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
		B = KMyMath::Vector3(end.x * t, end.y * t, end.z * t);

		KMyMath::Vector3 position;
		position = KMyMath::Vector3(A.x + B.x, A.y + B.y, A.z + B.z);
		return position;
	}

	float InQuintFloat(const float start, const float end, const float time)
	{
		float t = powf(time, 5);

		return start * (1.0f - t) + end * t;
	}

	float OutQuintFloat(const float start, const float end, const float time)
	{
		float t = (1 - powf(1 - time, 4));

		return start * (1.0f - t) + end * t;
	}

	float InOutQuintFloat(const float start, const float end, const float time)
	{
		float t;
		if (time < 0.5f)
		{
			t = 16 * powf(time, 5);
		}
		else
		{
			t = (1 - powf(-2 * time + 2, 5) / 2);
		}

		return start * (1.0f - t) + end * t;
	}

	KMyMath::Vector2 InQuintVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time)
	{
		float t = powf(time, 5);
		KMyMath::Vector2 A, B;
		A = KMyMath::Vector2(start.x * (1.0f - t), start.y * (1.0f - t));
		B = KMyMath::Vector2(end.x * t, end.y * t);

		KMyMath::Vector2 position;
		position = KMyMath::Vector2(A.x + B.x, A.y + B.y);
		return position;
	}

	KMyMath::Vector2 OutQuintVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time)
	{
		float t = (1 - powf(1 - time, 5));
		KMyMath::Vector2 A, B;
		A = KMyMath::Vector2(start.x * (1.0f - t), start.y * (1.0f - t));
		B = KMyMath::Vector2(end.x * t, end.y * t);

		KMyMath::Vector2 position;
		position = KMyMath::Vector2(A.x + B.x, A.y + B.y);
		return position;
	}

	KMyMath::Vector2 InOutQuintVec2(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time)
	{
		float t;
		if (time < 0.5f)
		{
			t = 16 * powf(time, 5);
		}
		else
		{
			t = (1 - powf(-2 * time + 2, 5) / 2);
		}

		KMyMath::Vector2 A, B;
		A = KMyMath::Vector2(start.x * (1.0f - t), start.y * (1.0f - t));
		B = KMyMath::Vector2(end.x * t, end.y * t);

		KMyMath::Vector2 position;
		position = KMyMath::Vector2(A.x + B.x, A.y + B.y);
		return position;
	}

	KMyMath::Vector3 InQuintVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time)
	{
		float t = powf(time, 5);
		KMyMath::Vector3 A, B;
		A = KMyMath::Vector3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
		B = KMyMath::Vector3(end.x * t, end.y * t, end.z * t);

		KMyMath::Vector3 position;
		position = KMyMath::Vector3(A.x + B.x, A.y + B.y, A.z + B.z);
		return position;
	}

	KMyMath::Vector3 OutQuintVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time)
	{
		float t = (1 - powf(1 - time, 5));
		KMyMath::Vector3 A, B;
		A = KMyMath::Vector3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
		B = KMyMath::Vector3(end.x * t, end.y * t, end.z * t);

		KMyMath::Vector3 position;
		position = KMyMath::Vector3(A.x + B.x, A.y + B.y, A.z + B.z);
		return position;
	}

	KMyMath::Vector3 InOutQuintVec3(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time)
	{
		float t;
		if (time < 0.5f)
		{
			t = 16 * powf(time, 5);
		}
		else
		{
			t = (1 - powf(-2 * time + 2, 5) / 2);
		}

		KMyMath::Vector3 A, B;
		A = KMyMath::Vector3(start.x * (1.0f - t), start.y * (1.0f - t), start.z * (1.0f - t));
		B = KMyMath::Vector3(end.x * t, end.y * t, end.z * t);

		KMyMath::Vector3 position;
		position = KMyMath::Vector3(A.x + B.x, A.y + B.y, A.z + B.z);
		return position;
	}
}