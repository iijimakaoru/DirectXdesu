#pragma once
#include "BaseScene.h"

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
	/// 次のシーン予約
	/// </summary>
	/// <param name="nextScene_"></param>
	void SetNestScene(BaseScene* nextScene_)
	{
		nextScene = nextScene_;
	}

	static SceneManager* GetInstance();

private:
	// 今のシーン
	BaseScene* scene = nullptr;
	// 次のシーン
	BaseScene* nextScene = nullptr;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	const SceneManager& operator=(const SceneManager&) = delete;
};

