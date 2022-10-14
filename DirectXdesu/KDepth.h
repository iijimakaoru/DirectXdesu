#pragma once
#include <d3d12.h>
#include "KDirectXCommon.h"
class KDepth
{
public:
	HRESULT result;

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

	KDepth(ID3D12Device* dev, KWinApp win);
	void SetResource(KWinApp win);
	void HeapDepth();
	void SetClwarDepth();
	void GeneResource(ID3D12Device* dev);
	void CreatDevHeap(ID3D12Device* dev);
	void CreatDepthView(ID3D12Device* dev);
};