#pragma once
#include "KDirectXCommon.h"
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

class SpriteCommon
{
public:
	void Init(KDirectXCommon* dxCommon);
	void Draw();
	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState() { return pipelineState; }
	Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature() { return rootSignature; }
	D3D12_VERTEX_BUFFER_VIEW GetVbView() { return vbView; }
	std::vector<DirectX::XMFLOAT3> GetVertices() { return vertices; }
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return dxCommon_->GetCmdlist().Get(); }

private:
	std::vector<DirectX::XMFLOAT3> vertices;

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
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	// 
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// 
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	// 
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState = nullptr;
};

