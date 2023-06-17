#pragma once
#include "BaseScene.h"

class SceneManager
{
public:
	~SceneManager();

	// 更新
	void Update();

	// 描画
	void Draw();

	/// <summary>
	/// 次のシーン予約
	/// </summary>
	/// <param name="nextScene_"></param>
	void SetNestScene(BaseScene* nextScene_)
	{
		nextScene = nextScene_;
	}

private:
	// 今のシーン
	BaseScene* scene = nullptr;
	// 次のシーン
	BaseScene* nextScene = nullptr;
};

