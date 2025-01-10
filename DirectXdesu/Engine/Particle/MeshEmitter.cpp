#include "MeshEmitter.h"

MeshEmitter::MeshEmitter(DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 acceleration,
	DirectX::XMFLOAT4 startColor,
	DirectX::XMFLOAT4 endColor) :
	velocity(velocity),
	acceleration(acceleration),
	startColor(startColor),
	endColor(endColor)
{

}

MeshEmitter::~MeshEmitter()
{
}

int MeshEmitter::GetEmitCount()
{
	return emitCount;
}

int MeshEmitter::GetVerticesPerParticle()
{
	return 1;
}

DirectX::XMFLOAT3 MeshEmitter::GetVelocity()
{
	return velocity;
}

DirectX::XMFLOAT3 MeshEmitter::GetAcceleration()
{
	return acceleration;
}

DirectX::XMFLOAT4 MeshEmitter::GetStartColor()
{
	return startColor;
}

DirectX::XMFLOAT4 MeshEmitter::GetEndColor()
{
	return endColor;
}
