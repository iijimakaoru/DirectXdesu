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

	// 次のシーンへのキー
	std::unique_ptr<Sprite> pushA = nullptr;
	TextureData pushATex;

	// オブジェクト
	std::unique_ptr<KObject3d> object3d;

	// モデル
	std::unique_ptr<KModel> model = nullptr;

	// パイプライン
	std::unique_ptr<KGPlin> objPipeline;
	KShader objShader;

	float nowAngle = 0;

	float easeTimer = 0;

	float easeTime = 30;

	bool goGame = false;
};

