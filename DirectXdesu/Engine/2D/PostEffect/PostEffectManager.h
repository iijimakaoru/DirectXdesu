#pragma once
#include "MultiPostEffect.h"
#include "SceneManager.h"
#include "TestPostEffect.h"
#include "VignettePostEffect.h"

class PostEffectManager {
public:
	void Init();

	void Update();

	void Draw();

	void PreDraw(SceneManager* scene);

	void PostDraw();

	enum PMode { Normal, Vignette };

	static void SetPostMode(const PMode& pMode);

public:
	static PMode postMode;

private:
	std::unique_ptr<TestPostEffect> testPostEffect_ = nullptr;
	std::unique_ptr<VignettePostEffect> vignettePostEffect_ = nullptr;
};
