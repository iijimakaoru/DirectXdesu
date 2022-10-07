#pragma once
#include "Sprite.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
#include <vector>
#include <string>
#include <DirectXTex.h>

class DebugText
{
public:
	// ’è”‚ÌéŒ¾
	static const int maxCharCount = 256;
	static const int fontWidth = 9;
	static const int fontHeight = 18;
	static const int fontLineCount = 14;
	// ƒƒ“ƒoŠÖ”‚ÌéŒ¾
	void Init(ID3D12Device* dev, int window_width, int window_height, UINT texnumber);
private:
	Sprite sprites[maxCharCount];
	int spriteIndex = 0;
};

