#include "Ease.h"

namespace MyEase
{
	const float Lerp(const float start, const float end, const float time)
	{
		return start * (1.0f - time) + end * time;
	}

	const KMyMath::Vector2 Lerp2D(const KMyMath::Vector2& start, const KMyMath::Vector2& end, const float time)
	{
		return start * (1.0f - time) + end * time;
	}

	const KMyMath::Vector3 Lerp3D(const KMyMath::Vector3& start, const KMyMath::Vector3& end, const float time)
	{
		return start * (1.0f - time) + end * time;
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