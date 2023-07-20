#pragma once
#include "Camera.h"
#include<memory>
#include "TextureManager.h"

class SceneManager;

class BaseScene
{
public: // メンバ関数
	virtual ~BaseScene() = default;

	// 初期化
	virtual void Init() = 0;

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

	// 終了
	virtual void Final() = 0;

	// リソース読み込み
	virtual void LoadResources() = 0;
	
	/// <summary>
	/// シーンマネージャーセット
	/// </summary>
	/// <param name="sceneManager_"></param>
	virtual void SetSceneManager(SceneManager* sceneManager_)
	{
		sceneManager = sceneManager_;
	}

protected:
	// シーンマネージャーを借りてくる
	SceneManager* sceneManager = nullptr;
};

