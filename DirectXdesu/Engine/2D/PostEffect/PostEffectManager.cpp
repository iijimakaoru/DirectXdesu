#include "PostEffectManager.h"

void PostEffectManager::Init() {
	testPostEffect_ = std::make_unique<TestPostEffect>();
	testPostEffect_->Init();

	vignettePostEffect_ = std::make_unique<VignettePostEffect>();
	vignettePostEffect_->Init();
}

void PostEffectManager::Draw() {
	testPostEffect_->Draw();

	//vignettePostEffect_->Draw();
}

void PostEffectManager::PreDraw() { 
	testPostEffect_->PreDrawScene();

	//vignettePostEffect_->PreDrawScene();
}

void PostEffectManager::PostDraw() {
	testPostEffect_->PostDrawScene();

	//vignettePostEffect_->PostDrawScene();
}
