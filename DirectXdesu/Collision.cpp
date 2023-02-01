#include "Collision.h"

using namespace DirectX;

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter)
{
	// À•WŒn‚ÌŒ´“_‚©‚ç‹…‚Ì’†SÀ•W‚Ö‚Ì‹——£
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
	// •½–Ê‚ÌŒ´“_‹——£‚ğŒ¸Z‚·‚é‚±‚Æ‚ÅA•½–Ê‚Æ‹…‚Ì’†S‚Æ‚Ì‹——£‚ªo‚é
	float dist = distV.m128_f32[0] - plane.distance;
	// ‹——£‚Ìâ‘Î’l‚ª”¼Œa‚æ‚è‘å‚«‚¯‚ê‚Î“–‚½‚Á‚Ä‚È‚¢
	if (fabsf(dist) > sphere.radius)
	{
		return false;
	}
	// ‹^—Œğ“_‚ğŒvZ
	if (inter)
	{
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}
