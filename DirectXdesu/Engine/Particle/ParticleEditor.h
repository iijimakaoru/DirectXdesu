#pragma once
#include "MyMath.h"

struct ParticleParameters
{
	int maxParticle;
	int gridSize;
	float emissionRate;
	float lifeTime;
	DirectX::XMFLOAT3 emitterPos;
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 acceleration;
	DirectX::XMFLOAT4 startColor;
	DirectX::XMFLOAT4 endColor;
};

class ParticleEditor
{
public:
	void Init();
	void Update();
	void Draw();

private:
	ParticleParameters particleParamaters_;
};

