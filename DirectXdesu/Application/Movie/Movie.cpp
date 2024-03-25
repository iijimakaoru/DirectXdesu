#include "Movie.h"
#include "PipelineManager.h"

BaseMovie::~BaseMovie() {}

void BaseMovie::Init() {
	for (size_t i = 0; i < 2; i++) {
		movieBar[i].reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));
	}
}

void BaseMovie::Draw() {
	for (size_t i = 0; i < 2; i++) {
		movieBar[0]->Draw(
		    TextureManager::GetInstance()->GetTextures("White1x1"), movieBarPos[0],
		    {static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW()), 50.0f}, 0.0f,
		    {0.0f, 0.0f, 0.0f, 1.0f}, false, false, {0.0f, 0.0f});

		movieBar[1]->Draw(
		    TextureManager::GetInstance()->GetTextures("White1x1"), movieBarPos[1],
		    {static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW()), 50.0f}, 0.0f,
		    {0.0f, 0.0f, 0.0f, 1.0f}, false, false, {0.0f, 1.0f});
	}
}

void BaseMovie::MovieBarInInit() {
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	movieBarPos[0] = {0.0f, 0.0f};
	movieBarPos[1] = {0.0f, height};
}

void BaseMovie::MovieBarOutInit() {
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	movieBarPos[0] = {0.0f, -50.0f};
	movieBarPos[1] = {0.0f, height + 50.0f};
}

void BaseMovie::MovieBarOut(const float timer_) {
	// ムービーバーを上下へ
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());
	movieBarPos[0] = MyEase::Lerp2D({0.0f, 0.0f}, {0.0f, -50.0f}, timer_);
	movieBarPos[1] = MyEase::Lerp2D({0.0f, height}, {0.0f, height + 50.0f}, timer_);
}

void BaseMovie::MovieBarIn(const float timer_) {
	// ムービーバーをにょっき
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());
	movieBarPos[0] = MyEase::Lerp2D({0.0f, -50.0f}, {0.0f, 0.0f}, timer_);
	movieBarPos[1] = MyEase::Lerp2D({0.0f, height + 50.0f}, {0.0f, height}, timer_);
}

const bool& BaseMovie::GetIsFinish() const { return isFinish_; }

void BaseMovie::SetIsFinish(bool isFinish) { isFinish_ = isFinish; }
