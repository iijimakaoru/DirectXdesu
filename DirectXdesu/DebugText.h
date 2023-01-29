#pragma once
#include "Sprite.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
#include <vector>
#include <string>
#include <DirectXTex.h>
#include "Sprite.h"
#include "Vector2.h"

class DebugText
{
public:
	// ’è”‚ÌéŒ¾
	static const int maxCharCount = 256;
	static const int fontWidth = 9;
	static const int fontHeight = 18;
	static const int fontLineCount = 14;
	// ƒƒ“ƒoŠÖ”‚ÌéŒ¾
	void Init(hogeSprite* sprite ,UINT texnumber, const HogeSpriteCommon& spriteCommon);
	void Print(const HogeSpriteCommon& spriteCommon, const std::string& text,
		Vector2 pos, float scale = 1.0f);
	void SpriteTransferVertexBuffer(const SpriteInfo& sprite, const HogeSpriteCommon& spriteCommon);
	void DrawAll(const HogeSpriteCommon& spriteCommon);
private:
	hogeSprite* sprite_ = nullptr;
	SpriteInfo sprites[maxCharCount];
	int spriteIndex = 0;
};

