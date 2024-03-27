#include "CollisionPrimitive.h"

using namespace DirectX;

void ColTriangle::ConputeNormal()
{
	KMyMath::Vector3 p0_p1 = p1 - p0; // p0からp1までのベクトル
	KMyMath::Vector3 p0_p2 = p2 - p0; // p0からp2までのベクトル

	// 外積により、2辺に垂直なベクトルを算出する
	normal = MyMathUtility::Vector3Cross(p0_p1, p0_p2);
	normal.Normalize();
}
