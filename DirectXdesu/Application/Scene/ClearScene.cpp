#include "ClearScene.h"

#include "DebugCamera.h"

#include "SceneManager.h"

#include "Ease.h"

#include <imgui.h>

#include "ScoreManager.h"
#include "PipelineManager.h"

ClearScene::~ClearScene() { Final(); }

void ClearScene::LoadResources() {
	// リザルトテクスチャ
	resultTex = TextureManager::Load("ResultText", ".png");

	// スコアボードテクスチャ
	scoreBordTex = TextureManager::Load("ScoreBord", ".png");

	// プッシュAテクスチャ
	pushATex = TextureManager::Load("kariNextScene", ".png");

	// 背景テクスチャ
	backTex = TextureManager::Load("ResultBack", ".png");

	gameScoreTex = TextureManager::Load("LevelScore", ".png");

	enemyScoreTex = TextureManager::Load("EnemyBonus", ".png");

	minDamageScoreTex = TextureManager::Load("MinDamageBonus", ".png");

	bossTimeScoreTex = TextureManager::Load("BossTimeBonus", ".png");

	totalTex = TextureManager::Load("Total", ".png");

	// 数字読み込み
	numTexs[0] = TextureManager::Load("Num0", ".png");
	numTexs[1] = TextureManager::Load("Num1", ".png");
	numTexs[2] = TextureManager::Load("Num2", ".png");
	numTexs[3] = TextureManager::Load("Num3", ".png");
	numTexs[4] = TextureManager::Load("Num4", ".png");
	numTexs[5] = TextureManager::Load("Num5", ".png");
	numTexs[6] = TextureManager::Load("Num6", ".png");
	numTexs[7] = TextureManager::Load("Num7", ".png");
	numTexs[8] = TextureManager::Load("Num8", ".png");
	numTexs[9] = TextureManager::Load("Num9", ".png");
}

void ClearScene::Init() {
	const float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	const float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	BaseScene::Init();

	input = KInput::GetInstance();

	camera = std::make_unique<DebugCamera>();

	sceneManager = SceneManager::GetInstance();

	// タイトル名
	result.reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));

	// スコアボード
	scoreBord.reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));

	// プッシュA
	pushA.reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));
	scoreBordSize = {0.0f, 0.01f};

	// 背景
	for (size_t i = 0; i < 2; i++) {
		back[i].reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));
	}

	backPos[0] = {width / 2.0f, height / 2.0f};
	backPos[1] = {backPos[0].x + width, backPos[0].y};

	gameScore.reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));
	for (size_t i = 0; i < 6; i++) {
		gameScoreS[i].reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));
	}

	enemyScore.reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));
	for (size_t i = 0; i < 6; i++) {
		enemyScoreS[i].reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));
	}

	minDamageScore.reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));
	for (size_t i = 0; i < 6; i++) {
		minDamageScoreS[i].reset(
		    Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));
	}

	bossTimeScore.reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));
	for (size_t i = 0; i < 6; i++) {
		bossTimeScoreS[i].reset(
		    Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));
	}

	total.reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));
	for (size_t i = 0; i < 6; i++) {
		totalS[i].reset(Sprite::Create(PipelineManager::GetInstance()->GetSpritePipeline()));
	}

	isGoScene = false;
}

void ClearScene::Update() {
	const float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	const float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	// テクスチャ場所
	resultPos.x = width / 2.0f;
	resultPos.y = height / 9.0f;
	scoreBordPos = {width / 2.0f, height * 5.0f / 9.0f};
	const float scoresTexPos = scoreBordPos.x - 500.0f;
	const float scoresNumPos = scoreBordPos.x + 500.0f;
	gameScorePos = {scoresTexPos, scoreBordPos.y - 150.0f};
	gameScoreSPos = {scoresNumPos, scoreBordPos.y - 150.0f};
	enemyScorePos = {scoresTexPos, scoreBordPos.y - 100.0f};
	enemyScoreSPos = {scoresNumPos, scoreBordPos.y - 100.0f};
	minDamageScorePos = {scoresTexPos, scoreBordPos.y - 50.0f};
	minDamageScoreSPos = {scoresNumPos, scoreBordPos.y - 50.0f};
	bossTimeScorePos = {scoresTexPos, scoreBordPos.y};
	bossTimeScoreSPos = {scoresNumPos, scoreBordPos.y};
	totalPos = {scoresTexPos, scoreBordPos.y + 150.0f};
	totalSPos = {scoresTexPos + 200.0f, scoreBordPos.y + 150.0f};
	pushAPos = {width / 2, height * 9 / 10};

	MoveBack();

	camera->Update();

	// 暗転待ち
	if (resultPhase == 0) {
		phaseTime = 30.0f;
		if (phaseTimer < phaseTime) {
			phaseTimer++;

		} else {
			phaseTimer = 0;
			resultPhase++;
		}
	}
	// リザルトボード出現(x)
	else if (resultPhase == 1) {
		phaseTime = 15.0f;
		if (phaseTimer < phaseTime) {
			phaseTimer++;

			scoreBordSize.x = MyEase::Lerp(0.0f, 1.0f, phaseTimer / phaseTime);
		} else {
			phaseTimer = 0;
			resultPhase++;
		}
	}
	// リザルトボード出現(y)
	else if (resultPhase == 2) {
		phaseTime = 15.0f;
		if (phaseTimer < phaseTime) {
			phaseTimer++;

			scoreBordSize.y = MyEase::Lerp(0.01f, 0.8f, phaseTimer / phaseTime);
		} else {
			phaseTimer = 0;
			resultPhase++;
		}
	}
	// スコアの描画ON
	else if (resultPhase == 3) {
		isDrawScores = true;
		resultPhase++;
	}
	// 待ち時間
	else if (resultPhase == 4) {
		phaseTime = 15.0f;
		if (phaseTimer < phaseTime) {
			phaseTimer++;
		} else {
			phaseTimer = 0;
			resultPhase++;
		}
	}
	// レベルスコア計算
	else if (resultPhase == 5) {
		size_t levelScore = ScoreManager::GetInstance()->GetGameScore();
		phaseTime = 30.0f;
		if (phaseTimer < phaseTime) {
			phaseTimer++;

			gameScoreNum = (size_t)MyEase::Lerp(0, (float)levelScore, phaseTimer / phaseTime);
		} else {
			phaseTimer = 0;
			resultPhase++;
		}
	}
	// 撃破スコア計算
	else if (resultPhase == 6) {
		float DestoryScore = 100000 * ScoreManager::GetInstance()->GetDestoryCount();
		phaseTime = 30.0f;
		if (phaseTimer < phaseTime) {
			phaseTimer++;

			enemyScoreNum = (size_t)MyEase::Lerp(0, DestoryScore, phaseTimer / phaseTime);
		} else {
			phaseTimer = 0;
			resultPhase++;
		}
	}
	// ダメージスコア計算
	else if (resultPhase == 7) {
		float DamageScore = 100000 * ScoreManager::GetInstance()->GetDamageCount();
		phaseTime = 30.0f;
		if (phaseTimer < phaseTime) {
			phaseTimer++;

			minDamageScoreNum = (size_t)MyEase::Lerp(0, DamageScore, phaseTimer / phaseTime);
		} else {
			phaseTimer = 0;
			resultPhase++;
		}
	}
	// 待ち時間
	else if (resultPhase == 7) {
		phaseTime = 15.0f;
		if (phaseTimer < phaseTime) {
			phaseTimer++;
		} else {
			phaseTimer = 0;
			resultPhase++;
		}
	}
	// 次のシーンへ
	else {
		isGoScene = true;
		// 次のシーンへ
		if (!sceneChange->GetIsEffect()) {
			if (input->IsTrigger(DIK_SPACE) || input->GetPadButtonDown(XINPUT_GAMEPAD_A)) {
				sceneChange->SceneChangeStart();
			}
		}
	}

	totalScoreNum = gameScoreNum + enemyScoreNum + minDamageScoreNum + bossTimeScoreNum;

	if (sceneChange->GetIsChange()) {
		// シーン切り替え依頼
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void ClearScene::ObjDraw() {}

void ClearScene::SpriteDraw() {
	for (size_t i = 0; i < 2; i++) {
		back[i]->Draw(backTex, backPos[i]);
	}

	result->Draw(resultTex, resultPos, {1, 1}, 0.0f, {1, 1, 1, 1}, false, false, {0.5f, 0});

	scoreBord->Draw(scoreBordTex, scoreBordPos, scoreBordSize);

	if (isDrawScores) {
		GameScoreDraw();

		EnemyScoreDraw();

		MinDamageScoreDraw();

		BossTimeScoreDraw();

		TotalScoreDraw();
	}

	if (isGoScene) {
		pushA->Draw(pushATex, pushAPos, {0.6f, 0.6f});
	}
}

void ClearScene::Final() {}

void ClearScene::MoveBack() {
	const float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());

	for (size_t i = 0; i < 2; i++) {
		if (backPos[i].x <= -width / 2) {
			if (i == 0) {
				backPos[0].x = backPos[1].x + width;
			} else {
				backPos[1].x = backPos[0].x + width;
			}
		}
		backPos[i].x--;
	}
}

void ClearScene::GameScoreDraw() {
	size_t scrNum = gameScoreNum;

	gameScore->Draw(
	    gameScoreTex, gameScorePos, {1, 1}, 0.0f, {1, 1, 1, 1}, false, false, {0.0f, 0.5f});

	size_t i = 0;
	while (i < 6) {
		KMyMath::Vector2 numsPos_ = gameScoreSPos;
		numsPos_.x = numsPos_.x - (15 * (i));
		size_t j = scrNum % 10;
		gameScoreS[i]->Draw(
		    numTexs[j], numsPos_, {1, 1}, 0.0f, {1, 1, 1, 1}, false, false, {1.0f, 0.5f});
		scrNum /= 10;
		i++;
	}
}

void ClearScene::EnemyScoreDraw() {
	size_t scrNum = enemyScoreNum;

	enemyScore->Draw(
	    enemyScoreTex, enemyScorePos, {1, 1}, 0.0f, {1, 1, 1, 1}, false, false, {0.0f, 0.5f});

	size_t i = 0;
	while (i < 6) {
		KMyMath::Vector2 numsPos_ = enemyScoreSPos;
		numsPos_.x = numsPos_.x - (15 * (i));
		size_t j = scrNum % 10;
		enemyScoreS[i]->Draw(
		    numTexs[j], numsPos_, {1, 1}, 0.0f, {1, 1, 1, 1}, false, false, {1.0f, 0.5f});
		scrNum /= 10;
		i++;
	}
}

void ClearScene::MinDamageScoreDraw() {
	size_t scrNum = minDamageScoreNum;

	minDamageScore->Draw(
	    minDamageScoreTex, minDamageScorePos, {1, 1}, 0.0f, {1, 1, 1, 1}, false, false,
	    {0.0f, 0.5f});

	size_t i = 0;
	while (i < 6) {
		KMyMath::Vector2 numsPos_ = minDamageScoreSPos;
		numsPos_.x = numsPos_.x - (15 * (i));
		size_t j = scrNum % 10;
		minDamageScoreS[i]->Draw(
		    numTexs[j], numsPos_, {1, 1}, 0.0f, {1, 1, 1, 1}, false, false, {1.0f, 0.5f});
		scrNum /= 10;
		i++;
	}
}

void ClearScene::BossTimeScoreDraw() {
	size_t scrNum = bossTimeScoreNum;

	bossTimeScore->Draw(
	    bossTimeScoreTex, bossTimeScorePos, {1, 1}, 0.0f, {1, 1, 1, 1}, false, false, {0.0f, 0.5f});

	size_t i = 0;
	while (i < 6) {
		KMyMath::Vector2 numsPos_ = bossTimeScoreSPos;
		numsPos_.x = numsPos_.x - (15 * (i));
		size_t j = scrNum % 10;
		bossTimeScoreS[i]->Draw(
		    numTexs[j], numsPos_, {1, 1}, 0.0f, {1, 1, 1, 1}, false, false, {1.0f, 0.5f});
		scrNum /= 10;
		i++;
	}
}

void ClearScene::TotalScoreDraw() {
	size_t scrNum = totalScoreNum;

	total->Draw(totalTex, totalPos, {1, 1}, 0.0f, {1, 1, 1, 1}, false, false, {0.0f, 0.5f});

	size_t i = 0;
	while (i < 6) {
		KMyMath::Vector2 numsPos_ = totalSPos;
		numsPos_.x = numsPos_.x - (15 * (i));
		size_t j = scrNum % 10;
		totalS[i]->Draw(
		    numTexs[j], numsPos_, {1, 1}, 0.0f, {1, 1, 1, 1}, false, false, {1.0f, 0.5f});
		scrNum /= 10;
		i++;
	}
}
