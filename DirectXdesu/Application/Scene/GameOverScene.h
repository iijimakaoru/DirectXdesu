#pragma once
#include "BaseScene.h"

#include "KInput.h"

#include "PostEffect.h"

#include "Sprite.h"

/**
 * @file GameOverScene.h
 * @brief ゲームオーバーシーン
 * @author 飯島 薫
 */

class GameOverScene : public BaseScene
{
public:
	GameOverScene() {};
	~GameOverScene();
	void LoadResources() override;
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

	// クリアスプライト
	std::unique_ptr<Sprite> over = nullptr;
	TextureData overTex;

	// 次のシーンへのキー
	std::unique_ptr<Sprite> pushA = nullptr;
	TextureData pushATex;
};

