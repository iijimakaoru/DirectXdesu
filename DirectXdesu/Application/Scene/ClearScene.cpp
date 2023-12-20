#include "ClearScene.h"

#include "DebugCamera.h"

#include "SceneManager.h"

#include "Ease.h"

#include <imgui.h>

ClearScene::~ClearScene() { Final(); }

void ClearScene::LoadResources() {
	// パイプライン
	spriteShader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	spritePipeline = std::make_unique<KGPlin>();
	spritePipeline->CreatePipelineAll(spriteShader, "Sprite");

	// リザルトテクスチャ
	resultTex = TextureManager::Load("Resources/texture/ResultText.png");

	// スコアボードテクスチャ
	scoreBordTex = TextureManager::Load("Resources/texture/ScoreBord.png");

	// プッシュAテクスチャ
	pushATex = TextureManager::Load("Resources/texture/kariNextScene.png");

	// 背景テクスチャ
	backTex = TextureManager::Load("Resources/texture/ResultBack.png");
}

void ClearScene::Init() {
	const float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	const float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	BaseScene::Init();

	input = KInput::GetInstance();

	camera = std::make_unique<DebugCamera>();

	sceneManager = SceneManager::GetInstance();

	// タイトル名
	result = std::make_unique<Sprite>();
	result->Init();
	result->SetPipeline(spritePipeline.get());

	// スコアボード
	scoreBord = std::make_unique<Sprite>();
	scoreBord->Init();
	scoreBord->SetPipeline(spritePipeline.get());

	// プッシュA
	pushA = std::make_unique<Sprite>();
	pushA->Init();
	pushA->SetPipeline(spritePipeline.get());
	scoreBordSize = {0.0f, 0.01f};

	// 背景
	for (size_t i = 0; i < 2; i++) {
		back[i] = std::make_unique<Sprite>();
		back[i]->Init();
		back[i]->SetPipeline(spritePipeline.get());
	}

	backPos[0] = {width / 2.0f, height / 2.0f};
	backPos[1] = {backPos[0].x + width, backPos[0].y};

	isGoScene = false;
}

void ClearScene::Update() {
	const float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	const float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	resultPos.x = width / 2.0f;
	resultPos.y = height / 9.0f;

	scoreBordPos = {width / 2.0f, height * 5.0f / 9.0f};

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
	// 待ち時間
	else if (resultPhase == 3) {
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
