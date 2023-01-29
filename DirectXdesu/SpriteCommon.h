#pragma once
#include "KGPlin.h"
#include "KShader.h"
#include "Matrix4.h"

class SpriteCommon
{
	KGPlin pipeline;
	KShader shader;

	Matrix mat2D;

	SpriteCommon();
	~SpriteCommon() {};
public:
	static SpriteCommon* GetInstance();

	SpriteCommon(const SpriteCommon& s) = delete;
	SpriteCommon& operator=(const SpriteCommon& s) = delete;

	void Init();
	void Draw();

	const Matrix& GetMat2D() const { return mat2D; }
};
