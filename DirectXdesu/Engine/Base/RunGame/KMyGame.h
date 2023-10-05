#pragma once
#include "Framework.h"

class KMyGame : public Framework
{
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

