#pragma once
#include "Sprite.h"

/**
 * @file Warning.h
 * @brief ボス戦導入の警告演出
 * @author 飯島 薫
 */

class Warning
{
public:
	// 初期化
	void Init();

	// 更新
	void Update();

	// 描画
	void Draw();

	// ゲッター
	bool GetIsDelete();
	int32_t GetTime();

private:
	// スプライト
	std::unique_ptr<Sprite> warning = nullptr;

	// 警告時間
	int32_t warningTimer = 0;
	int32_t warningTime = 0;

	// アルファ値
	float alpha = 0.0f;

	// アルファ値変更スピード
	float alphaSpeed = -0.03f;

	// 削除フラグ
	bool isDelete = false;
};

