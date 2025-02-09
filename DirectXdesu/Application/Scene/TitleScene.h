#pragma once
#include "KGPlin.h"
#include "KInput.h"

#include "TextureManager.h"
#include "Sprite.h"

#include "BaseScene.h"
#include "Camera.h"
#include "KObject3d.h"
#include "AudioManager.h"
#include "GameCamera.h"
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

	float RotationLogoY(const float& speed);

private:
	// インプット
	KInput* input = nullptr;

	// カメラ
	std::unique_ptr<GameCamera> camera = nullptr;

#pragma region リソース
	// オブジェクト
	std::unique_ptr<KObject3d> skyDome = nullptr;
	std::unique_ptr<KObject3d> logo = nullptr;

	// モデル
	KModel* skyDomeModel = nullptr;
	KModel* logoModel = nullptr;

	//テクスチャ
	std::unique_ptr<Sprite> backGround;
	std::unique_ptr<Sprite> pressA;
	TextureData texBG;
	TextureData texPressA;
#pragma endregion

	AudioManager* audioManager = nullptr;

	std::unique_ptr<Light> light_ = nullptr;

private:
	const float skydomeSize = 800.0f; 

	float rotationSpeed = 0.5f;
	float result = 0.0f;
	float timer = 0.0f;
	bool flag = true;
};

