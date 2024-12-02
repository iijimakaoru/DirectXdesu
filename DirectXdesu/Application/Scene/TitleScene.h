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
#include "Emitter.h"
#include "d3dUtil.h"
#include "FrameResource.h"
#include "Timer.h"

#include "GPUParticle.h"
#include "MeshGPUParticle.h"

#include "ParticleEditor.h"

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

	// We pack the UAV counter into the same buffer as the commands rather than create
	// a separate 64K resource/heap for it. The counter must be aligned on 4K boundaries,
	// so we pad the command buffer (if necessary) such that the counter will be placed
	// at a valid location in the buffer.
	static inline UINT AlignForUavCounter(UINT bufferSize)
	{
		const UINT alignment = D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT;
		return (bufferSize + (alignment - 1)) & ~(alignment - 1);
	}
private:
	// インプット
	KInput* input = nullptr;

	// カメラ
	std::unique_ptr<TitleCamera> camera = nullptr;

#pragma region プレイヤー機体
	// オブジェクト
	std::unique_ptr<KObject3d> object3d = nullptr;

	// モデル
	KModel* model = nullptr;
#pragma endregion

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

	Timer timer_;

	Emitter* emitter_;
	GPUParticle* gpuParticle_;
	MeshGPUParticle* meshGpuParticle_;

	ParticleEditor* particleEditor_;
};

