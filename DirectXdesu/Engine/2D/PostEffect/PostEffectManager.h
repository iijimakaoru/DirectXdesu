#pragma once
#include "TestPostEffect.h"
#include "VignettePostEffect.h"

class PostEffectManager {
public:
	void Init();

	void Draw();

	void PreDraw();

	void PostDraw();

private:
	std::unique_ptr<TestPostEffect> testPostEffect_ = nullptr;
	std::unique_ptr<VignettePostEffect> vignettePostEffect_ = nullptr;
};
