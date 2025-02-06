#pragma once
#include "Camera.h"
#include "KGPlin.h"
#include "BaseScene.h"
#include "KInput.h"
#include "PostEffect.h"
#include "Sprite.h"
#include "TitleCamera.h"
#include "AudioManager.h"
#include "KObject3d.h"

#include "d3dUtil.h"
#include "FrameResource.h"
#include "Timer.h"

#include "GPUParticle.h"
#include "MeshGPUParticle.h"

#include "Emitter.h"
#include "MeshEmitter.h"

#include "ArrowEffect.h"

/**
 * @file TitleScene.h
 * @brief タイトルシーン
 * @author 飯島 薫
 */

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

	// タイトル導入演出
	void StartScene();

	// 次のシーンへ
	void GoNextScene();

	// タイトルコール
	void TitleCall();
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

	KMyMath::Vector3 lightRGB = {1, 1, 1};
	KMyMath::Vector3 lightDir = {0, -1, 0};

	std::unique_ptr<Timer> timer_;

	Emitter* emitter_;

	std::unique_ptr<MeshModel> testM;
	std::unique_ptr<MeshGPUParticle> testP;

	// Imgui用
	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 rotation = { 0,0,0 };
	DirectX::XMFLOAT3 scaling = { 1,1,1 };

	// テスト用
	std::unique_ptr<Sprite> sprite;
	TextureData texData;
};

