#pragma once
#include <DirectXMath.h>
#include "KDirectXCommon.h"
#include <vector>
#include "ConstBuffer.h"
#include "KShader.h"
#include <memory>
#include <d3dx12.h>
#include "KSpriteCommon.h"

struct SpriteInfo
{
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//	頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// Z軸周りの回転角
	float rotation = 0.0f;
	// 座標
	Vector3 position = { 50,50,0 };
	// ワールド行列
	XMMATRIX matWorld;
	// 色
	XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
	// テクスチャ番号
	UINT texNum = 0;
	// サイズ
	Vector2 size = { 100,100 };
	// アンカーポイント
	Vector2 anchorpoint = { 0.5f,0.5f };
	// 左右反転
	bool isFlipX = false;
	// 上下反転
	bool isFlipY = false;
	// テクスチャ左上
	Vector2 texLeftTop = { 0,0 };
	// テクスチャ切り出しサイズ
	Vector2 texSize = { 100,100 };
	// 切り出しフラグ
	bool isKiridasi = false;
	// 非表示
	bool isInvisible = false;
};

class Sprite
{
public:
	void Init(SpriteCommon* spriteCommon);

	void SpriteTransferVertexBuffer();
	SpriteInfo SpriteCreate(Vector2 anchorpoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);

	void SpriteDraw();
	void SpriteUpdate();

private:
	SpriteCommon* spriteCommon_ = nullptr;

	SpriteInfo sprite_;
};

