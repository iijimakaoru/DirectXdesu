#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include "Camera.h"

struct Particle
{
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Position;
	float Age;
	DirectX::XMFLOAT3 Velocity;
	float Size;
	float Alive;
	DirectX::XMFLOAT3 Padding;
};

struct ParticleSort
{
	unsigned int index;
};

class Emitter
{
public:
	Emitter(int maxParticles,
		int gridSize,
		float emissionRate,
		float lifeTime,
		DirectX::XMFLOAT3 position,
		DirectX::XMFLOAT3 scaling,
		DirectX::XMFLOAT3 rotation,
		DirectX::XMFLOAT4 color,
		DirectX::XMFLOAT3 velocity,
		DirectX::XMFLOAT3 acceleration);
	~Emitter();

	int GetEmitCount();
	int GetMaxParticles();
	int GetGridSize();
	int GetVerticesPerParticle();
	float GetLifeTime();
	float GetEmitTimeCounter();
	float GetTimeBetweenEmit();
	DirectX::XMFLOAT3 GetVelocity();
	DirectX::XMFLOAT3 GetAcceleration();

	void SetEmitCount(int value);
	void SetEmitTimeCounter(float value);

	void Update(float deltaTime);

	DirectX::XMFLOAT3 GetPosition();
	void SetPosition(DirectX::XMFLOAT3& pos);
	DirectX::XMFLOAT3 GetRotation();
	void SetRotation(DirectX::XMFLOAT3& rot);
	DirectX::XMFLOAT3 GetScaling();
	void SetScaling(DirectX::XMFLOAT3& scale);
	DirectX::XMFLOAT4 GetColor();
	void SetColor(DirectX::XMFLOAT4& color);

private:
	//emitter settings
	int maxParticles;
	int gridSize;
	int emitCount;
	float lifeTime;
	float emissionRate;
	float timeBetweenEmit;
	float emitTimeCounter;
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 acceleration;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scaling;
	DirectX::XMFLOAT4 coloring;
};

