#include "MathHelper.h"
#include <float.h>
#include <cmath>

using namespace DirectX;

const float MathHelper::Infinity = FLT_MAX;
const float MathHelper::Pi = 3.1415926535f;

float MathHelper::AngleFromXY(float x, float y)
{
	float theta = 0.0f;

	// Quadrant I or IV
	if (x >= 0.0f)
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if (theta < 0.0f)
			theta += 2.0f * Pi; // in [0, 2*pi).
	}

	// Quadrant II or III
	else
		theta = atanf(y / x) + Pi; // in [0, 2*pi).

	return theta;
}

XMVECTOR MathHelper::RandUnitVec3()
{
	XMVECTOR One = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR Zero = XMVectorZero();

	// 半球上/半球内の点が得られるまで試し続けてください。
	while (true)
	{
		// 立方体 [-1,1]^3 内にランダムな点を生成します。
		XMVECTOR v = XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), 0.0f);

		// 均一な分布を得るために、単位球の外側の点を無視します。
		// 単位球上。  そうしないと、近くの球上にポイントがさらに集中してしまいます。
		// 立方体の角。

		if (XMVector3Greater(XMVector3LengthSq(v), One))
			continue;

		return XMVector3Normalize(v);
	}
}

XMVECTOR MathHelper::RandHemisphereUnitVec3(XMVECTOR n)
{
	XMVECTOR One = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR Zero = XMVectorZero();

	// 半球上/半球内の点が得られるまで試し続けてください。
	while (true)
	{
		// 立方体 [-1,1]^3 内にランダムな点を生成します。
		XMVECTOR v = XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), 0.0f);

		// 均一な分布を得るために、単位球の外側の点を無視します。
		// 単位球上。  そうしないと、近くの球上にポイントがさらに集中してしまいます。
		// 立方体の角。
		if (XMVector3Greater(XMVector3LengthSq(v), One))
			continue;

		// 下半球の点は無視します。
		if (XMVector3Less(XMVector3Dot(n, v), Zero))
			continue;

		return XMVector3Normalize(v);
	}
}