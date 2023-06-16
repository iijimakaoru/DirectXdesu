#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <d3dx12.h>
#include "KGPlin.h"
#include "KMaterial.h"
#include <wrl.h>
#include "KTexture.h"

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
	void Draw(KTexture* texture, bool isFlipX_ = false, bool isFlipY_ = false,
		DirectX::XMFLOAT2 anchorPoint_ = { 0.5f,0.5f }, DirectX::XMFLOAT2 setSize_ = { 100.0f,100.0f });

	/// <summary>
	/// パイプラインセッター
	/// </summary>
	/// <param name="pipeline_"></param>
	void SetPipeline(KGPlin* pipeline_);

private:
	// 定数バッファマテリアル
	void CreateCBMaterial();

	// 定数バッファトランスフォーム
	void CreateCBTransform();

private:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	enum VertexNumber
	{
		LB, // 左下
		LT, // 左上
		RB, // 右下
		RT, // 右上
	};

	struct Info
	{
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
		// 左右フリップ
		bool isFlipX = false;
		// 上下フリップ
		bool isFlipY = false;
		// 非表示
		bool isInvisible = false;
	};

	HRESULT result;

	ID3D12Device* device;

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

	// 頂点マップ
	Vertex* vertMap = nullptr;

	static KGPlin* pipeline;

public:
	Info info;
};