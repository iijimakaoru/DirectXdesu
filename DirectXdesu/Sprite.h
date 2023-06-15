#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <d3dx12.h>
#include "KGPlin.h"
#include "KMaterial.h"
#include <wrl.h>
#include "KTexture.h"

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

class SpriteCommon
{
private:
	
private:
	HRESULT result;

	ID3D12Device* device;

public:
	// 初期化
	void Init();

	// 更新
	void Update();

	// 描画
	void Draw();

	static SpriteCommon* GetInstance();

	/// <summary>
	/// パイプラインセッター
	/// </summary>
	/// <param name="pipeline_"></param>
	void SetPipeline(KGPlin* pipeline_);

private:
	KGPlin* pipeline;

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

	// 更新
	void Update();

	// 描画
	void Draw(KTexture* texture);

private:
	// 定数バッファマテリアル
	void CreateCBMaterial();

	// 定数バッファトランスフォーム
	void CreateCBTransform();

private:
	enum VertexNumber
	{
		LB, // 左下
		LT, // 左上
		RB, // 右下
		RT, // 右上
	};

	HRESULT result;

	ID3D12Device* device;

	SpriteCommon* spriteCommon = nullptr;

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	// 定数バッファのマップ
	ConstBufferDataMaterial* constMapMaterial = nullptr;

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;

	// 定数バッファのマップ
	KConstBufferDataTransform* constMapTransform = nullptr;

	// 頂点データ
	std::vector<Vertex> vertices;

	Vertex* vertMap = nullptr;

public:
	// 位置
	DirectX::XMFLOAT2 position = { 0.0f,0.0f };
	// 回転
	float rotation = 0.0f;
	// 色
	DirectX::XMFLOAT4 color = { 1,1,1,1 };
	// 表示サイズ
	DirectX::XMFLOAT2 size_ = { 100.0f,100.0f };
	// アンカーポイント
	DirectX::XMFLOAT2 anchorPoint = { 0.5f,0.5f };
};