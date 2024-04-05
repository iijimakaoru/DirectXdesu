#pragma once
#include "FbxLoader.h"
#include "KWinApp.h"
#include "KDirectXCommon.h"
#include "KInput.h"
#include "SceneManager.h"
#include "ImguiManager.h"
#include "ASceneFactory.h"
#include "PostEffect.h"
#include "TestPostEffect.h"
#include "VignettePostEffect.h"
#include "TextureManager.h"
#include "SceneChange.h"

#include "AudioManager.h"

/**
 * @file Framework.h
 * @brief フレームワーク
 * @author 飯島 薫
 */

class Framework
{
public:
	// 初期化
	virtual void Init();
	// 終了
	virtual void Final();
	// 更新
	virtual void Update();
	// 描画
	virtual void Draw() = 0;
	// 終了チェック
	virtual bool IsEndRwquest();

	virtual ~Framework() = default;

	// 実行
	void Run();

protected:
	// FBXローダー
	FbxLoader* fbxLoader = nullptr;

	// ウィンドウ
	KWinApp* win = nullptr;

	// DirectX
	KDirectXCommon* dx = nullptr;

	// Input
	KInput* input = nullptr;

	// シーンマネージャー
	SceneManager* sceneManager = nullptr;

	// テクスチャマネージャー
	TextureManager* textureManager = nullptr;

	// シーンファクトリー
	ASceneFactory* sceneFactory = nullptr;

	ImguiManager imguiMane;

	// ポストエフェクト
	std::unique_ptr<TestPostEffect> postEffect = nullptr;

	SceneChange* sceneChange = nullptr;

	AudioManager* audioManager = nullptr;
};

