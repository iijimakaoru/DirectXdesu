#pragma once
#include "BaseScene.h"

#include "KInput.h"

#include "PostEffect.h"

#include "Sprite.h"

/**
 * @file ClearScene.h
 * @brief ゲームクリアシーン
 * @author 飯島 薫
 */

class ClearScene : public BaseScene {
public:
	ClearScene(){};
	~ClearScene();
	void LoadResources() override;
	void Init() override;
	void Update() override;
	void ObjDraw() override;
	void SpriteDraw() override;
	void Final() override;

private:
	// 背景動かし
	void MoveBack();

private:
	// インプット
	KInput* input = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera = nullptr;

	// パイプライン
	std::unique_ptr<KGPlin> spritePipeline = nullptr;

	// シェーダー
	KShader spriteShader;

	// クリアスプライト
	std::unique_ptr<Sprite> result = nullptr;
	TextureData resultTex;
	KMyMath::Vector2 resultPos;

	// 次のシーンへのキー
	std::unique_ptr<Sprite> pushA = nullptr;
	TextureData pushATex;

	// スプライト背景
	std::array<std::unique_ptr<Sprite>, 2> back;
	TextureData backTex;
	std::array<KMyMath::Vector2, 2> backPos;
};
