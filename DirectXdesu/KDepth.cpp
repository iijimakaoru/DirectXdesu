#include "KDepth.h"

KDepth::KDepth(KDirectInit dx, KWindow win) {
	SetResource(win);
	HeapDepth();
	SetClwarDepth();
	GeneResource(dx);
	CreatDevHeap(dx);
	CreatDepthView(dx);
}

void KDepth::SetResource(KWindow win) {
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = win.window_width; // �����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = win.window_height; // �����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // �f�v�X�X�e���V��
}

void KDepth::HeapDepth() {
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
}

void KDepth::SetClwarDepth() {
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g
}

void KDepth::GeneResource(KDirectInit dx) {
	depthBuff = nullptr;
	dx.result = dx.dev->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));
}

void KDepth::CreatDevHeap(KDirectInit dx) {
	dsvHeapDesc.NumDescriptors = 1; // �[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // �f�v�X�X�e���V���r���[
	dsvHeap = nullptr;
	dx.result = dx.dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
}

void KDepth::CreatDepthView(KDirectInit dx) {
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dx.dev->CreateDepthStencilView(
		depthBuff,
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());
}