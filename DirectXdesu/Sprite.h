#pragma once
#include <DirectXMath.h>
#include "KDirectInit.h"
#include "Vector3.h"
#include "Vector2.h"
#include <vector>
#include "PipelineSet.h"
#include "ConstBuffer.h"
#include "KShader.h"
#include <memory>
#include <d3dx12.h>

const int spriteSRVCount = 512;

struct VertexPosUV
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

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
	Vector3 position = { 0,0,0 };
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
	// 非表示
	bool isInvisible = false;
};

struct SpriteCommon
{
	// パイプラインセット
	PipelineSet pipelineSet;
	// 射影行列
	XMMATRIX matProjection{};
	// テクスチャ用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// テクスチャリソース(テクスチャバッファ)の配列
	ComPtr<ID3D12Resource> texBuff[spriteSRVCount];
};

class Sprite
{
public:
	void Init(ID3D12Device* dev, int window_width, int window_height);
	void SpriteTransferVertexBuffer(const SpriteInfo& sprite, const SpriteCommon& spriteCommon);
	PipelineSet SpriteCreateGraphicsPipeline(ID3D12Device* dev);
	SpriteInfo SpriteCreate(ID3D12Device* dev, int window_width, int window_height, UINT texNumber, 
		const SpriteCommon& spriteCommon, Vector2 anchorpoint = { 0.5f,0.5f }, 
		bool isFlipX = false, bool isFlipY = false);
	void SpriteCommonBeginDraw(ID3D12GraphicsCommandList* cmdList, const SpriteCommon& spriteCommon);
	void SpriteDraw(const SpriteInfo& sprite, ID3D12GraphicsCommandList* cmdList,
		const SpriteCommon& spriteCommon, ID3D12Device* dev);
	SpriteCommon SpriteCommonCreate(ID3D12Device* dev, int window_width, int window_height);
	void SpriteUpdate(SpriteInfo& sprite, const SpriteCommon& spriteCommon);
	HRESULT SpriteCommonLoadTexture(SpriteCommon& spriteCommon,
		UINT texnumber, const wchar_t* filename, ID3D12Device* dev);

private:
	
};

