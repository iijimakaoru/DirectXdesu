#include "CollisionPrimitive.h"

using namespace DirectX;

void Triangle::ConputeNormal()
{
	XMVECTOR p0_p1 = p1 - p0; // p0からp1までのベクトル
	XMVECTOR p0_p2 = p2 - p0; // p0からp2までのベクトル

	// 外積により、2辺に垂直なベクトルを算出する
	normal = XMVector3Cross(p0_p1, p0_p2);
	normal = XMVector3Normalize(normal);
}
