#pragma once
#include "MyMath.h"
#include "KGPlin.h"
#include "KWinApp.h"

/**
 * @file PostEffect.h
 * @brief ポストエフェクト
 * @author 飯島 薫
 */

class PostEffect
{
public:// 静的メンバ関数
	//　静的初期化
	static void StaticInit();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 描画情報
	/// </summary>
	void DrawCommand();

	// シーン描画前処理
	void PreDrawScene();

	// シーン描画後処理
	void PostDrawScene();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// パイプラインセッター
	/// </summary>
	/// <param name="pipeline_"></param>
	void SetPipeline(KGPlin* pipeline_);

private:// クラス内でしか使わない関数
	// 頂点生成
	void CreateVertex();

	// テクスチャバッファ
	void CreateTextureBuff();

	// 深度バッファ
	void CreateDepthBuff();

	// DSV用デスクリプタヒープ
	void CreateDSVDescHeap();

	// RTV用デスクリプタヒープ
	void CreateRTVDescHeap();

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

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;

	// テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff = nullptr;

	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	// 深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff = nullptr;

	// RTV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;

	// DSV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	Vertex vertices_[4];

private:// 静的メンバ変数
	// デバイス
	static Microsoft::WRL::ComPtr<ID3D12Device> device;

	// コマンドリスト
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList;

	// パイプライン
	static KGPlin* pipeline;

	// ウィンドウ
	static KWinApp* window;

	// 画面クリアカラー
	static const float clearColor[4];
};

