#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "FbxObject3D.h"
#include "KDirectXCommon.h"
#include "KGPlin.h"
#include "KInput.h"
#include "KModel.h"
#include "KObject3d.h"
#include "KShader.h"
#include "ParticleManager.h"
#include "PostEffect.h"
#include "SkyBox.h"
#include "Sprite.h"
#include "Warning.h"
#include <array>
#include <imgui.h>

#include "AudioManager.h"

#include "Light.h"

#include "GameManager.h"
#include "Movie.h"
#include "CollisionManager.h"
#include "GameCamera.h"

/*
 * @file GameScene.h
 * @brief ゲームシーン
 * @author 飯島 薫
 */

class GameScene : public BaseScene {
public:
	GameScene(){};
	~GameScene();
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
	std::unique_ptr<GameCamera> camera_ = nullptr;

	// オーディオ
	AudioManager* audioManager_;

	// ライト
	std::unique_ptr<Light> light_ = nullptr;
	KMyMath::Vector3 lightRGB_ = {1, 1, 1};
	KMyMath::Vector3 lightDir_ = {0, -1, 0};

	std::unique_ptr<KModel> hogeModel_ = nullptr;
	std::unique_ptr<KObject3d> hogeObject_ = nullptr;
};
