#include "PostEffectManager.h"

PostEffectManager::PMode PostEffectManager::postMode = PostEffectManager::PMode::Normal;

void PostEffectManager::Init() {
	testPostEffect_ = std::make_unique<TestPostEffect>();
	testPostEffect_->Init();

	vignettePostEffect_ = std::make_unique<VignettePostEffect>();
	vignettePostEffect_->Init();
}

void PostEffectManager::Draw() {
	switch (postMode) {
	case PostEffectManager::Normal:
		testPostEffect_->Draw();
		break;
	case PostEffectManager::Vignette:
		vignettePostEffect_->Draw();
		break;
	default:
		break;
	}
}

void PostEffectManager::PreDraw(SceneManager* scene) { 
	switch (postMode) {
	case PostEffectManager::Normal:
		testPostEffect_->PreDrawScene();
		scene->Draw();
		testPostEffect_->PostDrawScene();
		break;
	case PostEffectManager::Vignette:
		vignettePostEffect_->PreDrawScene();
		scene->Draw();
		vignettePostEffect_->PostDrawScene();
		break;
	default:
		break;
	}
}

void PostEffectManager::PostDraw() {
	

	
}
