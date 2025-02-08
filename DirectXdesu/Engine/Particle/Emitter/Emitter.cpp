#include "Emitter.h"

Emitter::Emitter(int maxParticles, 
	int gridSize, 
	float emissionRate, 
	float lifeTime, 
	float size,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 scaling,
	DirectX::XMFLOAT3 rotation,
	DirectX::XMFLOAT4 startColor,
	DirectX::XMFLOAT4 endColor,
	DirectX::XMFLOAT3 velocity, 
	DirectX::XMFLOAT3 acceleration) :
	maxParticles(maxParticles),
	gridSize(gridSize),
	emissionRate(emissionRate),
	lifeTime(lifeTime),
	size(size),
	position(position),
	rotation(rotation),
	scaling(scaling),
	startColor(startColor),
	endColor(endColor),
	velocity(velocity),
	acceleration(acceleration)
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

DirectX::XMFLOAT4 Emitter::GetStartColor()
{
	return startColor;
}

void Emitter::SetStartColor(DirectX::XMFLOAT4& color)
{
	startColor = color;
}

DirectX::XMFLOAT4 Emitter::GetEndColor()
{
	return endColor;
}

void Emitter::SetEndColor(DirectX::XMFLOAT4& color)
{
	endColor = color;
}

float Emitter::GetParticleSize()
{
	return size;
}

void Emitter::SetParticleSize(float& size_)
{
	size = size_;
}
