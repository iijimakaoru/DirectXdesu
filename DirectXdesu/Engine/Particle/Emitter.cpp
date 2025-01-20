#include "Emitter.h"

Emitter::Emitter(int maxParticles, 
	int gridSize, 
	float emissionRate, 
	float lifeTime, 
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 scaling,
	DirectX::XMFLOAT3 rotation,
	DirectX::XMFLOAT3 velocity, 
	DirectX::XMFLOAT3 acceleration, 
	DirectX::XMFLOAT4 startColor, 
	DirectX::XMFLOAT4 endColor) :
	maxParticles(maxParticles),
	gridSize(gridSize),
	emissionRate(emissionRate),
	lifeTime(lifeTime),
	position(position),
	rotation(rotation),
	scaling(scaling),
	velocity(velocity),
	acceleration(acceleration),
	startColor(startColor),
	endColor(endColor)
{
	emitTimeCounter = 0.0f;
	timeBetweenEmit = 1.0f / emissionRate;
}

Emitter::~Emitter()
{
}

int Emitter::GetEmitCount()
{
	return emitCount;
}

int Emitter::GetMaxParticles()
{
	return maxParticles;
}

int Emitter::GetGridSize()
{
	return gridSize;
}

int Emitter::GetVerticesPerParticle()
{
	return 1;
}

float Emitter::GetLifeTime()
{
	return lifeTime;
}

float Emitter::GetEmitTimeCounter()
{
	return emitTimeCounter;
}

float Emitter::GetTimeBetweenEmit()
{
	return timeBetweenEmit;
}

DirectX::XMFLOAT3 Emitter::GetVelocity()
{
	return velocity;
}

DirectX::XMFLOAT3 Emitter::GetAcceleration()
{
	return acceleration;
}

DirectX::XMFLOAT4 Emitter::GetStartColor()
{
	return startColor;
}

DirectX::XMFLOAT4 Emitter::GetEndColor()
{
	return endColor;
}

void Emitter::SetEmitCount(int value)
{
	emitCount = value;
}

void Emitter::SetEmitTimeCounter(float value)
{
	emitTimeCounter = value;
}

void Emitter::Update(float deltaTime)
{
	emitTimeCounter += deltaTime;
}

DirectX::XMFLOAT3 Emitter::GetPosition()
{
	return position;
}

void Emitter::SetPosition(DirectX::XMFLOAT3& pos)
{
	position = pos;
}

DirectX::XMFLOAT3 Emitter::GetRotation()
{
	return rotation;
}

void Emitter::SetRotation(DirectX::XMFLOAT3& rot)
{
	rotation = rot;
}

DirectX::XMFLOAT3 Emitter::GetScaling()
{
	return scaling;
}

void Emitter::SetScaling(DirectX::XMFLOAT3& scale)
{
	scaling = scale;
}
