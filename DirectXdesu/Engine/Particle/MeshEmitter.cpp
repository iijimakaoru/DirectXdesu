#include "MeshEmitter.h"

MeshEmitter::MeshEmitter(float emissionRate, 
	float lifeTime,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 velocity,
	DirectX::XMFLOAT3 acceleration,
	DirectX::XMFLOAT4 startColor,
	DirectX::XMFLOAT4 endColor) :
	lifeTime(lifeTime),
	position(position),
	velocity(velocity),
	acceleration(acceleration),
	startColor(startColor),
	endColor(endColor)
{
	emitTimeCounter = 0.0f;
	timeBetweenEmit = 1.0f / emissionRate;
}

MeshEmitter::~MeshEmitter()
{
}

int MeshEmitter::GetEmitCount()
{
	return emitCount;
}

int MeshEmitter::GetMaxParticles()
{
	return 0;
}

int MeshEmitter::GetGridSize()
{
	return 0;
}

int MeshEmitter::GetVerticesPerParticle()
{
	return 1;
}

float MeshEmitter::GetLifeTime()
{
	return lifeTime;
}

float MeshEmitter::GetEmitTimeCounter()
{
	return 0.0f;
}

float MeshEmitter::GetTimeBetweenEmit()
{
	return 0.0f;
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

void MeshEmitter::SetEmitCount(int value)
{
	emitCount = value;
}

void MeshEmitter::SetEmitTimeCounter(float value)
{
	emitTimeCounter = value;
}

void MeshEmitter::Update(float deltaTime)
{
	emitTimeCounter += deltaTime;
}
