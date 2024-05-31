#include "PostEffectManager.h"

PostEffectManager::PMode PostEffectManager::postMode = PostEffectManager::PMode::Gaussian;

void PostEffectManager::Init() {
	testPostEffect_ = std::make_unique<TestPostEffect>();
	testPostEffect_->Init();

	vignettePostEffect_ = std::make_unique<VignettePostEffect>();
	vignettePostEffect_->Init();

	gaussianBlur_ = std::make_unique<GaussianBlur>();
	gaussianBlur_->Init();
}

void PostEffectManager::Update() { vignettePostEffect_->Update(); }

void PostEffectManager::Draw() {
	switch (postMode) {
	case PostEffectManager::Normal:
		testPostEffect_->Draw();
		break;
	case PostEffectManager::Vignette:
		vignettePostEffect_->Draw();
		break;
	case PostEffectManager::Gaussian:
		gaussianBlur_->Draw();
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
	case PostEffectManager::Gaussian:
		gaussianBlur_->PreDrawScene();
		scene->Draw();
		gaussianBlur_->PostDrawScene();
		break;
	default:
		break;
	}
}

void PostEffectManager::PostDraw() {}

void PostEffectManager::SetPostMode(const PMode& pMode) { postMode = pMode; }
