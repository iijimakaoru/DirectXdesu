#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <d3dx12.h>
#include "KGPlin.h"
#include "KMaterial.h"

const int spriteSRVCount = 512;

class SpriteCommon
{
private:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};
private:
	std::vector<Vertex> vertices;

	// 定数バッファ
	ID3D12Resource* constBuffMaterial = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// SRV最大数
	const size_t kMaxSRVCount = 2056;

	// 横方向ピクセル数
	const size_t textureWidth = 256;
	// 縦方向ピクセル数
	const size_t textureHeight = 256;
	// 配列の要素数
	const size_t imageDataCount = textureWidth * textureHeight;
	// 画像イメージデータ配列
	DirectX::XMFLOAT4* imageData = new DirectX::XMFLOAT4[imageDataCount]; // あとで解放する

public:
	// 初期化
	void Init();

	// 描画
	void Draw();

	static SpriteCommon* GetInstance();

	// ゲッター
	D3D12_VERTEX_BUFFER_VIEW& GetVbView();

private:
	SpriteCommon() = default;
	~SpriteCommon() = default;

	SpriteCommon(const SpriteCommon&) = default;
	const SpriteCommon& operator=(const SpriteCommon&) = delete;
};

class Sprite
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="spriteCommon"></param>
	void Init();

	// 描画
	void Draw();

	/// <summary>
	/// パイプラインセッター
	/// </summary>
	/// <param name="pipeline_"></param>
	void SetPipeline(KGPlin* pipeline_);

private:
	static KGPlin* pipeline;

	SpriteCommon* spriteCommon = nullptr;
};