#pragma once
#include "Camera.h"
#include "KGPlin.h"

#include "BaseScene.h"

#include "KInput.h"

#include "PostEffect.h"

#include "Sprite.h"
#include "TitleCamera.h"

#include "AudioManager.h"

#include "KObject3d.h"

/**
 * @file TitleScene.h
 * @brief タイトルシーン
 * @author 飯島 薫
 */

class TitleScene : public BaseScene
{
public:
	TitleScene(){};
	~TitleScene();
	void LoadResources()override;
	void Init() override;
	void Update() override;
	void ObjDraw() override;
	void SpriteDraw() override;
	void Final()override;

	// タイトル導入演出
	void StartScene();

	// 次のシーンへ
	void GoNextScene();

	// タイトルコール
	void TitleCall();
private:
	// インプット
	KInput* input = nullptr;

	// カメラ
	std::unique_ptr<TitleCamera> camera = nullptr;

#pragma region プレイヤー機体
	// オブジェクト
	std::unique_ptr<KObject3d> object3d = nullptr;

	// モデル
	KModel* model = nullptr;
#pragma endregion

#pragma region 天球
	// オブジェクト
	std::unique_ptr<KObject3d> skyDome = nullptr;

	// モデル
	KModel* skyDomeModel = nullptr;
#pragma endregion

	// 画面サイズ
	const float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	const float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

#pragma region タイトル始めの演出変数
	// シーン始まったフラグ
	bool startScene = false;

	// 演出のフェーズ
	uint32_t startScenePhase = 0;

	// ベジエ曲線１
	KMyMath::Vector3 start = { 0,-30,180 };
	KMyMath::Vector3 p1 = { 0,-30,20 };
	KMyMath::Vector3 p2 = { 0,-30,-40 };
	KMyMath::Vector3 end = { 0,60,-60 };

	// オブジェクト拡大タイマー
	float objEaseTimer = 0;
	const float objEaseTime = 20;

	// フラッシュ
	std::unique_ptr<Sprite> flash = nullptr;

	float flashAlpha = 0;

	// タイトル文字の演出
	uint32_t titlePhase = 0;
	float titlePhaseTimer = 0;
	float titlePhaseTime = 0;
	bool isTitle = false;
#pragma endregion

#pragma region 次のシーンへの移行演出変数
	// 次のシーンへいくフラグ
	bool goGame = false;

	// 演出のフェーズ
	uint32_t goGamePhase = 0;

	// テキスト吹き飛ばしタイマー
	float texEaseTimer = 0;
	const float texEaseTime = 10;
#pragma endregion

	// フェーズ時間
	float phaseTimer = 0;
	float phaseTime = 0;

	AudioManager* audioManager = nullptr;

	std::unique_ptr<Light> light_ = nullptr;

	KMyMath::Vector3 lightRGB = {1, 1, 1};
	KMyMath::Vector3 lightDir = {0, -1, 0};
};

