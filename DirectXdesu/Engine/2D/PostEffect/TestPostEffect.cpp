#include "TestPostEffect.h"

void TestPostEffect::Init() {
	BasePostEffect::Init();

	// パイプラインセット
	pipeline = pipelineManager->GetPipeline("PostEffect");
}

void TestPostEffect::DrawCommand() {
	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = {descHeapSRV.Get()};
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// SRVヒープの先頭ハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descHeapSRV->GetGPUDescriptorHandleForHeapStart();
	// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// 頂点バッファビューの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &vbView);
}

void TestPostEffect::PreDrawScene() {
	// リソースバリア変更
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
	    texBuff.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
	    D3D12_RESOURCE_STATE_RENDER_TARGET);
	cmdList->ResourceBarrier(1, &barrier);

	// レンダーターゲットビュー用デスクリプタヒープのハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = descHeapRTV->GetCPUDescriptorHandleForHeapStart();

	// 深度ステンシルビュー用デスクリプタヒープのハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();

	// レンダーターゲットをセット
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	// ビューポート設定
	CD3DX12_VIEWPORT viewPort = CD3DX12_VIEWPORT(
	    0.0f, 0.0f, static_cast<FLOAT>(window->GetWindowSizeW()),
	    static_cast<FLOAT>(window->GetWindowSizeH()));
	cmdList->RSSetViewports(1, &viewPort);

	// シザリング矩形設定
	CD3DX12_RECT rect = CD3DX12_RECT(
	    0, 0, static_cast<LONG>(window->GetWindowSizeW()),
	    static_cast<LONG>(window->GetWindowSizeH()));
	cmdList->RSSetScissorRects(1, &rect);

	// 背面クリア
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

	// 深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void TestPostEffect::PostDrawScene() {
	// リソースバリアを変更
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
	    texBuff.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET,
	    D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	cmdList->ResourceBarrier(1, &barrier);
}

void TestPostEffect::Draw() {
	// パイプラインセット
	pipeline->Setting();
	pipeline->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // 三角形リスト

	// 描画の条件
	DrawCommand();

	// 描画コマンド
	cmdList->DrawInstanced(_countof(vertices_), 1, 0, 0);
}
