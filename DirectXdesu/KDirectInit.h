#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
#include <vector>
#include <string>
#include"KWindow.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class KDirectInit
{
public:
	KDirectInit(KWindow window);
	void SetDXGIFactory();
	void SetAdapter();
	void SetDevice(IDXGIAdapter4* tmpAdapter);
	void SetCommandList();
	void SetCommandQueue();
	void SetSwapChain(KWindow window);
	void SetDescriptor();
	void SetBackBuffer();
	void SetFence();
	
	HRESULT result;
	ID3D12Device* dev = nullptr;
	IDXGIFactory6* dxgiFactory = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* cmdAllocater = nullptr;
	ID3D12GraphicsCommandList* cmdList = nullptr;
	ID3D12CommandQueue* cmdQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;
	float bRed = 0.1f;
	float bGreen = 0.25f;
	float bBule = 0.5f;

	// �Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[4] ={
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	D3D_FEATURE_LEVEL featureLevel;

	// �R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};

	// �X���b�v�`�F�[���̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	// �o�b�N�o�b�t�@
	std::vector<ID3D12Resource*> backBuffers;

	// �t�F���X�̐���
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;
};

