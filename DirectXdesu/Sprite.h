#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <d3dx12.h>
#include "KGPlin.h"
#include "KMaterial.h"
#include <wrl.h>
#include "KTexture.h"

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
	HRESULT result;

	ID3D12Device* device;

	std::vector<Vertex> vertices;

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;

public:
	// 初期化
	void Init();

	// 描画
	void Draw();

	static SpriteCommon* GetInstance();

private:
	// 定数バッファマテリアル
	void CreateCBMaterial();

	// 定数バッファトランスフォーム
	void CreateCBTransform();

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
	void Draw(KTexture* texture);

	/// <summary>
	/// パイプラインセッター
	/// </summary>
	/// <param name="pipeline_"></param>
	void SetPipeline(KGPlin* pipeline_);

private:
	static KGPlin* pipeline;

	SpriteCommon* spriteCommon = nullptr;
};