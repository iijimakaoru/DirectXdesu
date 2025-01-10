#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include "Camera.h"

class MeshEmitter
{
public:
	MeshEmitter(float emissionRate, 
		float lifeTime,
		DirectX::XMFLOAT3 position,
		DirectX::XMFLOAT3 velocity,
		DirectX::XMFLOAT3 acceleration,
		DirectX::XMFLOAT4 startColor,
		DirectX::XMFLOAT4 endColor);
	~MeshEmitter();

	int GetEmitCount();
	int GetMaxParticles();
	int GetGridSize();
	int GetVerticesPerParticle();
	float GetLifeTime();
	float GetEmitTimeCounter();
	float GetTimeBetweenEmit();
	DirectX::XMFLOAT3 GetVelocity();
	DirectX::XMFLOAT3 GetAcceleration();
	DirectX::XMFLOAT4 GetStartColor();
	DirectX::XMFLOAT4 GetEndColor();

	void SetEmitCount(int value);
	void SetEmitTimeCounter(float value);

	void Update(float deltaTime);

private:
	//emitter settings
	Transform transform;
	DirectX::XMFLOAT3 position;
	int maxParticles;
	int gridSize;
	int emitCount;
	float lifeTime;
	float emissionRate;
	float timeBetweenEmit;
	float emitTimeCounter;
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 acceleration;
	DirectX::XMFLOAT4 startColor;
	DirectX::XMFLOAT4 endColor;
};

