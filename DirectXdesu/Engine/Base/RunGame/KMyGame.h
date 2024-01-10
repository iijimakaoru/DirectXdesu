#pragma once
#include "Framework.h"

/**
 * @file KMyGame.h
 * @brief ゲーム内処理順の基底クラス
 * @author 飯島 薫
 */

namespace MesiEngine {
class KMyGame : public Framework {
public:
	// 初期化
	void Init() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 終了
	void Final() override;

private:
};
}

