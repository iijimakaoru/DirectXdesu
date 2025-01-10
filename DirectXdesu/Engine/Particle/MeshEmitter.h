#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include "Camera.h"

class MeshEmitter
{
public:
	MeshEmitter(DirectX::XMFLOAT3 velocity,
		DirectX::XMFLOAT3 acceleration,
		DirectX::XMFLOAT4 startColor,
		DirectX::XMFLOAT4 endColor);
	~MeshEmitter();

	int GetEmitCount();
	int GetVerticesPerParticle();
	DirectX::XMFLOAT3 GetVelocity();
	DirectX::XMFLOAT3 GetAcceleration();
	DirectX::XMFLOAT4 GetStartColor();
	DirectX::XMFLOAT4 GetEndColor();

private:
	//emitter settings

	int maxParticles;
	int emitCount;
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 acceleration;
	DirectX::XMFLOAT4 startColor;
	DirectX::XMFLOAT4 endColor;
};

