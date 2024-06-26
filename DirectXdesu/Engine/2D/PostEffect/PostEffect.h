#pragma once
#include "MyMath.h"
#include "KGPlin.h"
#include "KWinApp.h"
#include "PipelineManager.h"

/**
 * @file PostEffect.h
 * @brief ポストエフェクト
 * @author 飯島 薫
 */

class BasePostEffect
{
public:// 静的メンバ関数
	// 静的初期化
	static void StaticInit();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 描画情報
	/// </summary>
	virtual void DrawCommand();

	// シーン描画前処理
	virtual void PreDrawScene();

	// シーン描画後処理
	virtual void PostDrawScene();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

private:// クラス内でしか使わない関数
	// 頂点生成
	void CreateVertex();

	// テクスチャバッファ
	void CreateTextureBuff();

	// 深度バッファ
	void CreateDescHeap();

	// DSV用デスクリプタヒープ
	void CreateDSVDescHeap();

	// RTV用デスクリプタヒープ
	void CreateRTVDescHeap();

protected:
	struct Vertex
	{
		KMyMath::Vector3 pos;
		KMyMath::Vector2 uv;
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

	// パイプライン
	KGPlin* pipeline = nullptr;

	// 画面クリアカラー
	float clearColor[4];

protected:// 静的メンバ変数
	// デバイス
	static Microsoft::WRL::ComPtr<ID3D12Device> device;

	// コマンドリスト
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList;

	// ウィンドウ
	static KWinApp* window;

	static PipelineManager* pipelineManager;
};

