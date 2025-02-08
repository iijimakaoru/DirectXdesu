#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "KGPlin.h"
#include "BaseScene.h"
#include "KInput.h"
#include "PostEffect.h"
#include "Sprite.h"
#include "TitleCamera.h"
#include "AudioManager.h"
#include "KObject3d.h"


class SettingScene :public BaseScene
{

public:
	SettingScene() {};
	~SettingScene();
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
	std::unique_ptr<TitleCamera> camera = nullptr;

#pragma region 天球
	// オブジェクト
	std::unique_ptr<KObject3d> skyDome = nullptr;

	// モデル
	KModel* skyDomeModel = nullptr;
#pragma endregion

	// 画面サイズ
	const float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	const float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	AudioManager* audioManager = nullptr;

	std::unique_ptr<Light> light_ = nullptr;

	KMyMath::Vector3 lightRGB = { 1, 1, 1 };
	KMyMath::Vector3 lightDir = { 0, -1, 0 };

};

