#pragma once
#include "FbxLoader.h"
#include "KWinApp.h"
#include "KDirectXCommon.h"
#include "KInput.h"
#include "SceneManager.h"
#include "ImguiManager.h"
#include "ASceneFactory.h"

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
	FbxLoader* fbxLoader = nullptr;

	KWinApp* win = nullptr;

	KDirectXCommon* dx = nullptr;

	KInput* input = nullptr;

	SceneManager* sceneManager = nullptr;

	// シーンファクトリー
	ASceneFactory* sceneFactory = nullptr;

	ImguiManager imguiMane;
};

