#include "ScoreManager.h"
#include "Ease.h"
#include "ImguiManager.h"

ScoreManager* ScoreManager::GetInstance() {
	static ScoreManager instance;
	return &instance;
}

void ScoreManager::Init() {
	// Sprite
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline.reset(KGPlin::Create(spriteShader, "Sprite"));

	for (size_t i = 0; i < 6; i++)
	{
		nums[i] = std::make_unique<Sprite>();
		nums[i]->Init();
		nums[i]->SetPipeline(spritePipeline.get());
	}

	numTexs[0] = TextureManager::Load("Resources/texture/Num0.png");
	numTexs[1] = TextureManager::Load("Resources/texture/Num1.png");
	numTexs[2] = TextureManager::Load("Resources/texture/Num2.png");
	numTexs[3] = TextureManager::Load("Resources/texture/Num3.png");
	numTexs[4] = TextureManager::Load("Resources/texture/Num4.png");
	numTexs[5] = TextureManager::Load("Resources/texture/Num5.png");
	numTexs[6] = TextureManager::Load("Resources/texture/Num6.png");
	numTexs[7] = TextureManager::Load("Resources/texture/Num7.png");
	numTexs[8] = TextureManager::Load("Resources/texture/Num8.png");
	numTexs[9] = TextureManager::Load("Resources/texture/Num9.png");
}

void ScoreManager::Update() {
	/*ImGui::Begin("TextPos");
	ImGui::SetWindowPos({10, 10});
	ImGui::SetWindowSize({200, 200});
	ImGui::SliderInt("Result.y", &(int)gameScore, 0, 100);
	ImGui::End();*/

	if (isAddScore)
	{
		if (addScoreTimer < addScoreTime) {
			addScoreTimer++;
			gameScore = (size_t)MyEase::Lerp(
			    (float)oldGameScore, (float)(oldGameScore + addScoreNum),
			    addScoreTimer / addScoreTime);
		} else {
			addScoreNum = 0;
			isAddScore = false;
		}
	} else {
		oldGameScore = gameScore;
	}
}

void ScoreManager::Draw() {
	size_t scrNum = gameScore;
	size_t i = 0;

	while (i < 6)
	{
		KMyMath::Vector2 pos = {800.0f, 50.0f};
		pos.x = pos.x - (15 * (i));
		size_t j = scrNum % 10;
		nums[i]->Draw(numTexs[j], pos, {1, 1}, 0.0f, {1, 1, 1, 1}, false, false, {0.5f, 0.5f});
		scrNum /= 10;
		i++;
	}
}

void ScoreManager::AddMobScore(size_t score_) { 
	addScoreNum = score_;
	addScoreTimer = 0;
	isAddScore = true;
}

void ScoreManager::AddBossScore(size_t score_) { 
	addScoreNum = score_; 
}
