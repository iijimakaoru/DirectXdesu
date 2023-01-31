#include "SpriteCommon.h"

SpriteCommon::SpriteCommon()
{
	Init();
}

SpriteCommon* SpriteCommon::GetInstance()
{
	static SpriteCommon instance;
	return &instance;
}

void SpriteCommon::Init()
{
	Vector3 vertices[] =
	{
		{-0.5f,-0.5f,0.0f},
		{-0.5f,+0.5f,0.0f},
		{+0.5f,-0.5f,0.0f},
	};
}

void SpriteCommon::Draw()
{
	pipeline.Setting();
	pipeline.Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


