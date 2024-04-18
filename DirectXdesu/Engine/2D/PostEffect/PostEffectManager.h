#pragma once
#include "TestPostEffect.h"
#include "VignettePostEffect.h"
#include "MultiPostEffect.h"
#include "SceneManager.h"

class PostEffectManager {
public:
	void Init();

	void Draw();

	void PreDraw(SceneManager* scene);

	void PostDraw();

private:
	std::unique_ptr<TestPostEffect> testPostEffect_ = nullptr;
	std::unique_ptr<VignettePostEffect> vignettePostEffect_ = nullptr;
	std::unique_ptr<MultiPostEffect> multiPostEffect_ = nullptr;
};
