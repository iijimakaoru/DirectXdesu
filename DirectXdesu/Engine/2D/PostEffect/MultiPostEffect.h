#pragma once
#include "KGPlin.h"
#include "KWinApp.h"
#include "MyMath.h"
#include "PipelineManager.h"

class MultiPostEffect {
public:
	void Init();

	void PreDrawScene(uint32_t num);

	void DrawCommand();

	void Draw();

	void PostDrawScene(uint32_t num);

private:
	// 頂点生成
	void CreateVertex();

	// テクスチャバッファ
	void CreateTextureBuff();

	// 深度バッファ
	void CreateDepthBuff();

	void CreateDescHeap();

	// DSV用デスクリプタヒープ
	void CreateDSVDescHeap();

	// RTV用デスクリプタヒープ
	void CreateRTVDescHeap();

private:
	struct Vertex {
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
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[2];

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

private: // 静的メンバ変数
	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device;

	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList;

	// ウィンドウ
	KWinApp* window;

	PipelineManager* pipelineManager;
};
