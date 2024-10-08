#pragma once
#include "BaseScene.h"

#include "KInput.h"

#include "PostEffect.h"

#include "Sprite.h"
#include "AudioManager.h"

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
	// インプット
	KInput* input = nullptr;

	// カメラ
	std::unique_ptr<Camera> camera = nullptr;
};
