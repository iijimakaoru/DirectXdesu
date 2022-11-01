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
	// 定数の宣言
	static const int maxCharCount = 256;
	static const int fontWidth = 9;
	static const int fontHeight = 18;
	static const int fontLineCount = 14;
	// メンバ関数の宣言
	void Init(ID3D12Device* dev, int window_width, int window_height,
		UINT texnumber, const SpriteCommon& spriteCommon);
	void Print(const SpriteCommon& spriteCommon, const std::string& text,
		Vector2 pos, float scale = 1.0f);
	void SpriteTransferVertexBuffer(const SpriteInfo& sprite, const SpriteCommon& spriteCommon);
	void DrawAll(ID3D12Device* dev, const SpriteCommon& spriteCommon,
		ID3D12GraphicsCommandList* cmdList);
private:
	std::unique_ptr<Sprite> sprite;
	SpriteInfo sprites[maxCharCount];
	int spriteIndex = 0;
};

