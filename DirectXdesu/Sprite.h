#pragma once
#include <DirectXMath.h>
#include "KDirectXCommon.h"
#include <vector>
#include "ConstBuffer.h"
#include "KShader.h"
#include <memory>
#include <d3dx12.h>
#include "KSpriteCommon.h"

class Sprite
{
public:
	void Init(SpriteCommon* spriteCommon);

	void SpriteTransferVertexBuffer(const SpriteInfo& sprite, const SpriteCommond& spriteCommon);
	SpriteInfo SpriteCreate(UINT texNumber,
		const SpriteCommond& spriteCommon, Vector2 anchorpoint = { 0.5f,0.5f },
		bool isFlipX = false, bool isFlipY = false);

	void SpriteDraw(const SpriteInfo& sprite, ID3D12GraphicsCommandList* cmdList, const SpriteCommond& spriteCommon);
	void SpriteUpdate(SpriteInfo& sprite, const SpriteCommond& spriteCommon);

private:
	SpriteCommon* spriteCommon_ = nullptr;
};

