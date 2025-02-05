#pragma once
#include "Camera.h"
#include "KGPlin.h"
#include "BaseScene.h"
#include "KInput.h"
#include "PostEffect.h"
#include "Sprite.h"
#include "GameCamera.h"
#include "AudioManager.h"
#include "KObject3d.h"

#include "FrameResource.h"
#include "Timer.h"

#include "ParticleEditor.h"

#include "Emitter.h"
#include "MeshEmitter.h"

/**
 * @file TitleScene.h
 * @brief タイトルシーン
 * @author 飯島 薫
 */

class TitleScene : public BaseScene
{
public:
	TitleScene() = default;
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
	std::unique_ptr<GameCamera> camera = nullptr;

#pragma region 天球
	// オブジェクト
	std::unique_ptr<KObject3d> skyDome = nullptr;
	std::unique_ptr<KObject3d> logo = nullptr;

	// モデル
	KModel* skyDomeModel = nullptr;
	KModel* logoModel = nullptr;
#pragma endregion

	AudioManager* audioManager = nullptr;

	std::unique_ptr<Light> light_ = nullptr;

	

private:
	const float skydomeSize = 800.0f; 
};

