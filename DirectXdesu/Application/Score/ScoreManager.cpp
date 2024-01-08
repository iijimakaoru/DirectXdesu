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

	// スコアスプライトズ
	for (size_t i = 0; i < 6; i++) {
		score[i] = std::make_unique<Sprite>();
		score[i]->Init();
		score[i]->SetPipeline(spritePipeline.get());
	}

	// 加算スコアスプライトズ
	for (size_t i = 0; i < 4; i++) {
		addScore[i] = std::make_unique<Sprite>();
		addScore[i]->Init();
		addScore[i]->SetPipeline(spritePipeline.get());
	}

	// ボーナスカウントスプライトズ
	for (size_t i = 0; i < 2; i++) {
		bonusCountS[i] = std::make_unique<Sprite>();
		bonusCountS[i]->Init();
		bonusCountS[i]->SetPipeline(spritePipeline.get());
	}

	// 数字読み込み
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

	// 「Score」文字
	scores = std::make_unique<Sprite>();
	scores->Init();
	scores->SetPipeline(spritePipeline.get());
	scoresTex = TextureManager::Load("Resources/texture/ScoreTex.png");

	// 「HIT」文字
	hits = std::make_unique<Sprite>();
	hits->Init();
	hits->SetPipeline(spritePipeline.get());
	hitsTex = TextureManager::Load("Resources/texture/HitTex.png");

	// 「X」文字
	xS = std::make_unique<Sprite>();
	xS->Init();
	xS->SetPipeline(spritePipeline.get());
	xTex = TextureManager::Load("Resources/texture/XTex.png");

	// ボーナス時間バースプライト
	bonusBar = std::make_unique<Sprite>();
	bonusBar->Init();
	bonusBar->SetPipeline(spritePipeline.get());
	bonusBarTex = TextureManager::Load("Resources/texture/scoreBonusBar.png");

	// ボーナス時間ゲージスプライト
	bonusGage = std::make_unique<Sprite>();
	bonusGage->Init();
	bonusGage->SetPipeline(spritePipeline.get());
	bonusGageTex = TextureManager::Load("Resources/texture/white1x1.png");
}

void ScoreManager::Update() {
	int GameScore_ = (int)gameScore;
	int DestoryCount_ = (int)destoryCount;
	int DestoryCountMax_ = (int)destoryCountMax;
	int DamageCount_ = (int)damageCount;
	int DamageCountMax_ = (int)damageCountMax;
	ImGui::Begin("ScoreCount");
	ImGui::SetWindowPos({10, 10});
	ImGui::SetWindowSize({200, 200});
	ImGui::Text("%d", GameScore_);
	ImGui::Text("%d", DestoryCount_);
	ImGui::Text("%d", DestoryCountMax_);
	ImGui::Text("%d", DamageCount_);
	ImGui::Text("%d", DamageCountMax_);
	ImGui::End();

	if (isCount) {
		if (bonusTimer > 0) {
			CountBonusTimer();
		} else {
			addResultScore = addScoreNum * bonusCount;
			addScoreNum = 0;
			bonusTimer = 0;
			bonusCount = 0;
			isBonus = false;
			isAddScore = true;
			isCount = false;
		}
	}

	if (isAddScore) {
		if (addScoreTimer < addScoreTime) {
			addScoreTimer++;
			gameScore = (size_t)MyEase::Lerp(
			    (float)oldGameScore, (float)(oldGameScore + addResultScore),
			    addScoreTimer / addScoreTime);
		} else {
			addScoreNum = 0;
			addScoreTimer = 0;
			addResultScore = 0;
			isAddScore = false;
		}
	} else {
		oldGameScore = gameScore;
	}
}

void ScoreManager::Draw() {
	GameScoreDraw();

	// bonus中
	if (isCount) {

		AddScoreDraw();

		BonusCountDraw();

		hits->Draw(
		    hitsTex, {scoresPos.x + 40.0f, numsPos.y + 40.0f}, {1.0f, 1.0f}, 0.0f,
		    {1.0f, 1.0f, 1.0f, 1.0f}, false, false, {0.0f, 0.5f});

		xS->Draw(xTex, {scoresPos.x + 102.5f, numsPos.y + 41.0f}, {0.8f, 0.8f});

		BonusTimerDraw();
	}
}

void ScoreManager::ResetScore() {
	// ゲームスコア
	gameScore = 0;
	// 過去ゲームスコア
	oldGameScore = 0;
	// 加算スコア
	addScoreNum = 0;
	// 加算スコア結果
	addResultScore = 0;
	isCount = false;
	isBonus = false;
	// ボーナスタイマー
	bonusTimer = 0;
	// ボーナスカウント
	bonusCount = 0;
	isAddScore = false;
	addScoreTimer = 0;
	// エネミー撃破カウント
	destoryCount = 0;
	destoryCountMax = 0;
}

void ScoreManager::AddMobScore(size_t score_) {
	addScoreNum += score_;
	bonusCount++;
	isCount = true;
	bonusTimer = bonusTime;
}

void ScoreManager::AddBossScore(size_t score_) {
	addResultScore = score_;
	isAddScore = true;
}

const size_t& ScoreManager::GetGameScore() const { return gameScore; }

void ScoreManager::CountBonusTimer() {
	// ボーナスフラグON
	isBonus = true;
	// ボーナス時間減少
	bonusTimer--;
}

void ScoreManager::GameScoreDraw() {
	size_t scrNum = gameScore;

	scores->Draw(
	    scoresTex, scoresPos, {1.0f, 1.0f}, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f}, false, false,
	    {0.0f, 0.0f});

	size_t i = 0;
	while (i < 6) {
		KMyMath::Vector2 numsPos_ = numsPos;
		numsPos_.x = numsPos_.x - (15 * (i));
		size_t j = scrNum % 10;
		score[i]->Draw(numTexs[j], numsPos_, {1, 1}, 0.0f, {1, 1, 1, 1});
		scrNum /= 10;
		i++;
	}
}

void ScoreManager::AddScoreDraw() {
	size_t addScrNum = addScoreNum;
	size_t i = 0;
	while (i < 4) {
		KMyMath::Vector2 numsPos_ = {numsPos.x, numsPos.y + 40.0f};
		numsPos_.x = numsPos_.x - (15 * (i));
		size_t j = addScrNum % 10;
		addScore[i]->Draw(numTexs[j], numsPos_, {1, 1}, 0.0f, {1, 1, 1, 1});
		addScrNum /= 10;
		i++;
	}
}

void ScoreManager::BonusCountDraw() {
	size_t bonusCountNum = bonusCount;
	size_t i = 0;
	while (i < 2) {
		KMyMath::Vector2 numsPos_ = {scoresPos.x + 25.0f, numsPos.y + 40.0f};
		numsPos_.x = numsPos_.x - (15 * (i));
		size_t j = bonusCountNum % 10;
		bonusCountS[i]->Draw(numTexs[j], numsPos_, {1, 1}, 0.0f, {1, 1, 1, 1});
		bonusCountNum /= 10;
		i++;
	}
}

void ScoreManager::BonusTimerDraw() {
	const KMyMath::Vector2 gageTexSize = {218.0f, 16.0f};

	const float sizeX = gageTexSize.x / bonusTime;
	const float sizeY = gageTexSize.y;

	const KMyMath::Vector2 bonusBarPos = {scoresPos.x + 103.5f, numsPos.y + 80.0f};

	bonusBar->Draw(bonusBarTex, bonusBarPos);

	const KMyMath::Vector2 bonusGagePos = {bonusBarPos.x - (gageTexSize.x / 2), bonusBarPos.y};

	bonusGage->Draw(
	    bonusGageTex, bonusGagePos, {bonusTimer * sizeX, sizeY}, 0.0f, {0, 0, 1, 1}, false, false,
	    {0.0f, 0.5f});
}

void ScoreManager::AddDestoryCountMax() { destoryCountMax++; }

void ScoreManager::AddDestoryCount() { destoryCount++; }

const float ScoreManager::GetDestoryCount() const {
	float result = 0;

	if (destoryCount == destoryCountMax) {
		result = 1;
	} else if (destoryCount >= destoryCountMax * 0.75f) {
		result = 0.75f;
	} else if (destoryCount >= destoryCountMax * 0.5f) {
		result = 0.5f;
	} else if (destoryCount >= destoryCountMax * 0.25f) {
		result = 0.25f;
	}

	return result;
}

const float ScoreManager::GetDamageCount() const {
	float result = 1;

	if (damageCount == damageCountMax) {
		result = 0;
	} else if (damageCount >= damageCountMax * 0.75f) {
		result = 0.25f;
	} else if (damageCount >= damageCountMax * 0.5f) {
		result = 0.5f;
	} else if (damageCount >= damageCountMax * 0.25f) {
		result = 0.75f;
	}

	return result;
}

void ScoreManager::SetDamageCountMax(size_t max_) { damageCountMax = max_; }

void ScoreManager::AddDamageCount() { damageCount++; }
