#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <d3dx12.h>
#include "KGPlin.h"

const int spriteSRVCount = 512;

class SpriteCommon
{
private:
	std::vector<DirectX::XMFLOAT3> vertices;

	// 頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

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