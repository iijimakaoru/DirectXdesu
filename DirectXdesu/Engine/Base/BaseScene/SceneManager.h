#pragma once
#include "BaseScene.h"
#include "ASceneFactory.h"

class SceneManager
{
public:
	// 更新
	void Update();

	// 描画
	void Draw();

	// 終了
	void Final();

	/// <summary>
	/// 次のシーン
	/// </summary>
	/// <param name="sceneName"></param>
	void ChangeScene(const std::string& sceneName);

	/// <summary>
	/// シーンファクトリーのセッター
	/// </summary>
	/// <param name="sceneFactory_"></param>
	void SetSceneFactory(ASceneFactory* sceneFactory_)
	{
		sceneFactory = sceneFactory_;
	}

	static SceneManager* GetInstance();

private:
	// 今のシーン
	BaseScene* scene = nullptr;
	// 次のシーン
	BaseScene* nextScene = nullptr;

	// シーンファクトリー
	ASceneFactory* sceneFactory = nullptr;

	SceneManager(){};
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	const SceneManager& operator=(const SceneManager&) = delete;
};

