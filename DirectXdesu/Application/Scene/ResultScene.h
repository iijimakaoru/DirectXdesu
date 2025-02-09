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

class ResultScene : public BaseScene
{
public:
	ResultScene() = default;
	~ResultScene() = default;

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
	std::unique_ptr<GameCamera> camera = nullptr;
	//ライト
	std::unique_ptr<Light> light_ = nullptr;

#pragma region リソース
	// オブジェクト
	std::unique_ptr<KObject3d> skydome = nullptr;
	std::unique_ptr<KObject3d> fox = nullptr;
	std::unique_ptr<KObject3d> stage = nullptr;

	// モデル
	KModel* skydomeModel = nullptr;
	KModel* foxModel = nullptr;
	KModel* stageModel = nullptr;

	//テクスチャ

#pragma endregion

private:
	const float skydomeSize = 800.0f; 
};