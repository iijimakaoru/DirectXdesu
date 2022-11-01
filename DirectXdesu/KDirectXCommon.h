#pragma once
#include <d3dx12.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
#include <vector>
#include"KWinApp.h"
#include "KWinApp.h"
#include "KDepth.h"
#include <chrono>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

class KDirectXCommon
{
public:
	KDirectXCommon();
	~KDirectXCommon()
	{
		
	}
	void Init(KWinApp* win);

	void PreDraw();

	void PostDraw();

	void CmdFlash();

	void CmdClear();

	// �f�o�C�X�擾
	Microsoft::WRL::ComPtr<ID3D12Device> GetDev() const { return dev; }
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCmdlist() { return cmdList; }

private:
	HRESULT result;
	// DirectX12�f�o�C�X
	Microsoft::WRL::ComPtr<ID3D12Device> dev;
	// DXGI�t�@�N�g���[
	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory;
	// �o�b�N�o�b�t�@
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;
	// �X���b�v�`�F�[��
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	// �[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff{};
	//�@�[�x�r���[�p�q�[�v�쐬
	ComPtr<ID3D12DescriptorHeap> dsvHeap{};
	ComPtr<ID3D12DescriptorHeap> tmpDsvHeap{};

	ComPtr<ID3D12CommandAllocator> cmdAllocater;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;

	D3D_FEATURE_LEVEL featureLevel;

	// �R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};

	// �X���b�v�`�F�[���̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// �t�F���X�̐���
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
	D3D12_RESOURCE_BARRIER barrierDesc{};

	float bRed = 0.1f;
	float bGreen = 0.25f;
	float bBule = 0.5f;

	KWinApp* win = nullptr;

	//KDepth* depth = nullptr;

	std::chrono::steady_clock::time_point reference_;
	// �f�o�C�X������
	void InitDev();
	// �R�}���h������
	void InitCommand();
	// �X���b�v�`�F�[��������
	void InitSwapChain();
	// �����_�[�^�[�Q�b�g�r���[������
	void InitRenderTargetView();
	// �[�x�o�b�t�@������
	void InitDepthBuffer();
	// �t�F���X������
	void InitFence();
	// FPS�Œ菉����
	void InitFixFPS();
	// FPS�Œ�
	void UpdateFixFPS();
};

