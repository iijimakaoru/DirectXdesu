#pragma once
#include "KDirectXCommon.h"
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

class SpriteCommon
{
public:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};
	void Init(KDirectXCommon* dxCommon);
	void Draw();
	ID3D12PipelineState* GetPipelineState() { return pipelineState_.Get(); }
	ID3D12RootSignature* GetRootSignature() { return rootSignature_.Get(); }
	D3D12_VERTEX_BUFFER_VIEW GetVbView() { return vbView_; }
	std::vector<Vertex> GetVertices() { return vertices_; }
	ID3D12GraphicsCommandList* GetCmdList() { return dxCommon_->GetCmdlist().Get(); }

private:
	const size_t textureWidth = 256;
	const size_t textureHeight = 256;

	const size_t imageDataCount = textureWidth * textureHeight;

	const size_t kMaxSRVCount = 2056;

	std::vector<Vertex> vertices_;
	std::vector<short> indices_;

	KDirectXCommon* dxCommon_ = nullptr;
	// 
	D3D12_HEAP_PROPERTIES heapProp_{};
	// 
	D3D12_RESOURCE_DESC resDesc_{};
	// 
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_ = nullptr;
	// 
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc_{};
	// 
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	// 
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc_{};
	// 
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_ = nullptr;
	// 
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial_ = nullptr;
	// 
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff_ = nullptr;
	// 
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_;
};

