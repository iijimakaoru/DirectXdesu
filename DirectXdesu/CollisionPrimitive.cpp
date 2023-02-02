#include "CollisionPrimitive.h"

using namespace DirectX;

void Triangle::ConputeNormal()
{
	XMVECTOR p0_p1 = p1 - p0; // p0����p1�܂ł̃x�N�g��
	XMVECTOR p0_p2 = p2 - p0; // p0����p2�܂ł̃x�N�g��

	// �O�ςɂ��A2�ӂɐ����ȃx�N�g�����Z�o����
	normal = XMVector3Cross(p0_p1, p0_p2);
	normal = XMVector3Normalize(normal);
}
