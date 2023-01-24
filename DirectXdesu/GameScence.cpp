#include "GameScence.h"

#include "ParticleManager.h"

void GameScence::LoadResouce()
{
	// モデル
	triangle = std::make_unique<Triangle>();
	cube = std::make_unique<Cube>();

	// テクスチャ
	mario->CreateTexture("Resources/texture/", "mario.jpg");
	haikei->CreateTexture("Resources/texture/", "haikei.jpg");
}

void GameScence::Init()
{
	
}

void GameScence::Update()
{
}

void GameScence::Draw()
{
}
