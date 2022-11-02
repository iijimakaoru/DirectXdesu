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
	ID3D12PipelineState* GetPipelineState() { return pipelineState.Get(); }
	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
	D3D12_VERTEX_BUFFER_VIEW GetVbView() { return vbView; }
	std::vector<Vertex> GetVertices() { return vertices; }
	ID3D12GraphicsCommandList* GetCmdList() { return dxCommon_->GetCmdlist().Get(); }

private:
	std::vector<Vertex> vertices;
	std::vector<short> indices;

	KDirectXCommon* dxCommon_ = nullptr;
	// 
	D3D12_HEAP_PROPERTIES heapProp{};
	// 
	D3D12_RESOURCE_DESC resDesc{};
	// 
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
	// 
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// 
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// 
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	// 
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	// 
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
};

