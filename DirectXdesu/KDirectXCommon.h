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
	void Init();

	void PreDraw();

	void PostDraw();

	void CmdFlash();

	void CmdClear();

	// �f�o�C�X�擾
	ID3D12Device* GetDev() const { return dev.Get(); }
	ID3D12GraphicsCommandList* GetCmdlist() { return cmdList.Get(); }

	float fps = 0;

private:
	HRESULT result;
	// DirectX12�f�o�C�X
	Microsoft::WRL::ComPtr<ID3D12Device> dev;
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

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	ComPtr<ID3D12CommandAllocator> cmdAllocater;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;

	// �R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};

	// �t�F���X�̐���
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
	D3D12_RESOURCE_BARRIER barrierDesc{};

	float bRed = 0.1f;
	float bGreen = 0.25f;
	float bBule = 0.5f;

	//KDepth* depth = nullptr;

	std::chrono::steady_clock::time_point reference_;

	// FPS�Œ菉����
	void InitFixFPS();
	// FPS�Œ�
	void UpdateFixFPS();

public:
	static KDirectXCommon* GetInstance();

private:
	KDirectXCommon() = default;
	~KDirectXCommon() = default;
	KDirectXCommon(const KDirectXCommon&) = default;
	const KDirectXCommon& operator=(const KDirectXCommon&) = delete;
};

