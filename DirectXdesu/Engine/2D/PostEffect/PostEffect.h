#pragma once
#include "Sprite.h"
class PostEffect : public Sprite
{
public:
	// コンストラクタ
	PostEffect();

	/// <summary>
	/// 描画コマンド
	/// </summary>
	void Draw();
};

