#include "PostEffectManager.h"

void PostEffectManager::Init() {
	multiPostEffect_ = std::make_unique<MultiPostEffect>();
	multiPostEffect_->Init();

	testPostEffect_ = std::make_unique<TestPostEffect>();
	testPostEffect_->Init();

	vignettePostEffect_ = std::make_unique<VignettePostEffect>();
	vignettePostEffect_->Init();
}

void PostEffectManager::Draw() {
	multiPostEffect_->Draw();

	//vignettePostEffect_->Draw();
}

void PostEffectManager::PreDraw(SceneManager* scene) { 
	testPostEffect_->PreDrawScene();
	scene->Draw();
	testPostEffect_->PostDrawScene();

	vignettePostEffect_->PreDrawScene();
	scene->Draw();
	vignettePostEffect_->PostDrawScene();

	multiPostEffect_->PreDrawScene(0);
	testPostEffect_->Draw();
	multiPostEffect_->PostDrawScene(0);

	multiPostEffect_->PreDrawScene(1);
	vignettePostEffect_->Draw();
	multiPostEffect_->PostDrawScene(1);
}

void PostEffectManager::PostDraw() {
	

	
}
