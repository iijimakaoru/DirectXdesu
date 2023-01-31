#pragma once
#include <d3d12.h>
#include "KGPlin.h"
#include "KShader.h"
#include "Matrix4.h"

class SpriteCommon
{
public:
	KGPlin pipeline;
	KShader shader;

	SpriteCommon();
	~SpriteCommon() {};
public:
	static SpriteCommon* GetInstance();

	SpriteCommon(const SpriteCommon& s) = delete;
	SpriteCommon& operator=(const SpriteCommon& s) = delete;

	void Init();
	void Draw();
};
