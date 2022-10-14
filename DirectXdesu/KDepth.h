#pragma once
#include <d3d12.h>
#include "KDirectXCommon.h"
class KDepth
{
public:
	KDepth(ID3D12Device* devint, int window_width, int window_height);
	ID3D12DescriptorHeap* GetDevHeap() { return dsvHeap; }

private:
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

	void SetResource(int window_width, int window_height);
	void HeapDepth();
	void SetClwarDepth();
	void GeneResource(ID3D12Device* dev);
	void CreatDevHeap(ID3D12Device* dev);
	void CreatDepthView(ID3D12Device* dev);
};