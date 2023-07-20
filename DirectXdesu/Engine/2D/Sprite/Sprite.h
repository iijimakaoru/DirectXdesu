#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <d3dx12.h>
#include "KGPlin.h"
#include <wrl.h>
#include "KTexture.h"

#include "MyMath.h"

#include "TextureManager.h"

class Sprite
{
public:
	static void StaticInit();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="spriteCommon"></param>
	void Init();

	// 更新
	void Update(KMyMath::Vector2 pos, KMyMath::Vector2 scale, float rot, KMyMath::Vector4 color);

	/// <summary>
	/// 描画情報
	/// </summary>
	/// <param name="texture"></param>
	void DrawCommand(TextureData texData);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="texture"></param>
	/// <param name="pos"></param>
	/// <param name="setSize_"></param>
	/// <param name="rot"></param>
	/// <param name="color"></param>
	/// <param name="isFlipX_"></param>
	/// <param name="isFlipY_"></param>
	/// <param name="anchorPoint_"></param>
	void Draw(TextureData& texData, KMyMath::Vector2 pos = { 0.0f,0.0f }, KMyMath::Vector2 setSize_ = { 100.0f,100.0f }, float rot = 0.0f,
		KMyMath::Vector4 color = {1.0f,1.0f,1.0f,1.0f}, bool isFlipX_ = false, bool isFlipY_ = false, KMyMath::Vector2 anchorPoint_ = { 0.5f,0.5f });

	/// <summary>
	/// パイプラインセッター
	/// </summary>
	/// <param name="pipeline_"></param>
	void SetPipeline(KGPlin* pipeline_);

	// ポジションゲッター
	const KMyMath::Vector2 GetPos() const;

private:
	// 定数バッファマテリアル
	void CreateCBMaterial();

	// 頂点、インデックス生成
	void CreateVertexIndex();

	// 定数バッファトランスフォーム
	void CreateCBTransform();

private:
	struct Vertex
	{
		KMyMath::Vector3 pos;
		KMyMath::Vector2 uv;
	};

	enum VertexNumber
	{
		LB, // 左下
		LT, // 左上
		RB, // 右下
		RT, // 右上
	};

	HRESULT result;

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;

	// 頂点マップ
	Vertex* vertMap = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;

	// インデックスマップ
	uint16_t* indexMap = nullptr;

	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	// 定数バッファのマップ
	KMyMath::Vector4* constMapMaterial = nullptr;

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;

	// 定数バッファのマップ
	KMyMath::Matrix4* constMapTransform = nullptr;

	// 位置
	KMyMath::Vector2 position = { 0.0f,0.0f };
	// 回転
	float rotation = 0.0f;
	// 色
	KMyMath::Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	// 表示サイズ
	KMyMath::Vector2 size_ = { 100.0f,100.0f };
	// アンカーポイント
	KMyMath::Vector2 anchorPoint = { 0.5f,0.5f };
	// 左右フリップ
	int flipX = 1;
	// 上下フリップ
	int flipY = 1;
	// 非表示
	bool isInvisible = false;

	// デバイス
	static Microsoft::WRL::ComPtr<ID3D12Device> device;

	// コマンドリスト
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList;

	static KGPlin* pipeline;

	static KMyMath::Matrix4 matPro;
};