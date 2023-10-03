#pragma once
#include "Camera.h"
#include "KGPlin.h"

#include "BaseScene.h"

#include "KInput.h"

#include "PostEffect.h"

#include "Sprite.h"

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

	// 次のシーンへ
	void GoNextScene();
private:
	// インプット
	KInput* input = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera = nullptr;

	// パイプライン
	std::unique_ptr<KGPlin> spritePipeline = nullptr;

	// シェーダー
	KShader spriteShader;

	// タイトルスプライト
	std::unique_ptr<Sprite> titleName = nullptr;
	TextureData titleTex;

	KMyMath::Vector2 titlePos = { 0,0 };

	// 次のシーンへのキー
	std::unique_ptr<Sprite> pushA = nullptr;
	TextureData pushATex;

	KMyMath::Vector2 pushAPos = { 0,0 };

	// オブジェクト
	std::unique_ptr<KObject3d> object3d;

	// モデル
	std::unique_ptr<KModel> model = nullptr;

	// パイプライン
	std::unique_ptr<KGPlin> objPipeline;
	KShader objShader;

	// 画面サイズ
	const float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	const float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

#pragma region タイトル始めの演出変数
	// シーン始まったフラグ
	bool startScene = false;

	// 演出のフェーズ
	uint32_t startScenePhase = 0;

	// フェーズ時間
	float phaseTimer = 0;

	float phaseTime = 0;
#pragma endregion

#pragma region 次のシーンへの移行演出変数
	// 次のシーンへいくフラグ
	bool goGame = false;

	// 演出のフェーズ
	uint32_t goGamePhase = 0;

	// 機体の今の角度
	float nowAngle = 0;

	// 角度補正イージングタイマー
	float easeTimer = 0;
	const float easeTime = 30;

	// テキスト吹き飛ばしタイマー
	float texEaseTimer = 0;
	const float texEaseTime = 10;

	// 機体飛び出しタイマー
	float objEaseTimer = 0;
	const float objEaseTime = 15;
#pragma endregion
};

