#pragma once
#include <d3d12.h>
#include "KGPlin.h"
#include "KShader.h"
#include "Matrix4.h"

class AhoSpriteCommon
{
public:
	KGPlin pipeline;
	KShader shader;

	AhoSpriteCommon();
	~AhoSpriteCommon() {};
public:
	static AhoSpriteCommon* GetInstance();

	AhoSpriteCommon(const AhoSpriteCommon& s) = delete;
	AhoSpriteCommon& operator=(const AhoSpriteCommon& s) = delete;

	void Init();
	void Draw();
};
