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
	void Update();
	void Draw();

private:
	SpriteCommon* spriteCommon_ = nullptr;
	// 
	D3D12_VERTEX_BUFFER_VIEW vbView{};
};

