#pragma once
#include "KDirectXCommon.h"
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>
#include "Vector3.h"
#include "Vector2.h"
#include "PipelineSet.h"

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

struct SpriteCommond
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

class SpriteCommon
{
public:
	void Init(KDirectXCommon* dxCommon);
	void Draw();

	ID3D12GraphicsCommandList* GetCmdList() { return dxCommon_->GetCmdlist().Get(); }

	void SpriteTransferVertexBuffer(const SpriteInfo& sprite, const SpriteCommond& spriteCommon);
	PipelineSet SpriteCreateGraphicsPipeline();
	SpriteInfo SpriteCreate(UINT texNumber,
		const SpriteCommond& spriteCommon, Vector2 anchorpoint = { 0.5f,0.5f },
		bool isFlipX = false, bool isFlipY = false);
	void SpriteCommonBeginDraw(ID3D12GraphicsCommandList* cmdList, const SpriteCommond& spriteCommon);
	void SpriteDraw(const SpriteInfo& sprite, ID3D12GraphicsCommandList* cmdList, const SpriteCommond& spriteCommon);
	SpriteCommond SpriteCommonCreate();
	void SpriteUpdate(SpriteInfo& sprite, const SpriteCommond& spriteCommon);
	HRESULT SpriteCommonLoadTexture(SpriteCommond& spriteCommon, UINT texnumber, const wchar_t* filename);

private:
	KDirectXCommon* dxCommon_ = nullptr;

};

