#pragma once
#include <d3d12.h>
#include "KDirectInit.h"
class KDepth
{
public:
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResourceDesc{};

	// �[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};

	// �[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};

	// �[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};

	// �[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

	ID3D12Resource* depthBuff{};

	ID3D12DescriptorHeap* dsvHeap{};

	KDepth(KDirectInit dx, KWindow win);
	void SetResource(KWindow win);
	void HeapDepth();
	void SetClwarDepth();
	void GeneResource(KDirectInit dx);
	void CreatDevHeap(KDirectInit dx);
	void CreatDepthView(KDirectInit dx);
};