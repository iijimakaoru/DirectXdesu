#pragma once
#include <d3dx12.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
#include <vector>
#include "KDepth.h"
#include <chrono>

#include "KRtvDescriptorHeap.h"
#include "KRenderTargetBuffer.h"
#include "KDescriptorHeap.h"
#include "KDepthStencilBuffer.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class KDirectXCommon
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	HRESULT result;
	char PADING[4];

	// DirectX12�f�o�C�X
	ComPtr<ID3D12Device> dev;

	// DXGI
	ComPtr<IDXGIFactory6> dxgiFactory;

	// �X���b�v�`�F�[��
	ComPtr<IDXGISwapChain4> swapChain;

	// �R�}���h
	ComPtr<ID3D12CommandAllocator> cmdAllocater;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandQueue> cmdQueue;

	// �t�F���X�̐���
	ComPtr<ID3D12Fence> fence;

	// RTV�p�̃f�X�N���v�^�q�[�v
	std::unique_ptr<KRtvDescriptorHeap> rtvHeap;

	// SRV�p�̃f�X�N���v�^�q�[�v
	std::unique_ptr<KDescriptorHeap> srvHeap;

	// �[�x�o�b�t�@
	std::unique_ptr<KDepthStencilBuffer> depthBuff{};

	// DSV�p�̃f�X�N���v�^�q�[�v
	std::unique_ptr<KDsvDescriptorHeap> dsvHeap{};

	// �o�b�N�o�b�t�@
	std::vector<std::unique_ptr<KRenderTargetBuffer>> backBuffers;

	// �t�F���X�̐���
	UINT64 fenceVal = 0;

	// �X���b�v�`�F�[���̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// �N���A�J���[
	FLOAT clearColor[4] = { 0.1f,0.25f,0.5f,0.0f };

	//�r���[�|�[�g
	D3D12_VIEWPORT viewport{};

	//�V�U�[��`
	D3D12_RECT scissorRect{};

	UINT bbIndex;

	char PADING2[4]{};

	static KDirectXCommon* directXCommon_;

	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	ComPtr<ID3D12DescriptorHeap> tmpDsvHeap{};

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	D3D_FEATURE_LEVEL featureLevel;

	// �R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};

	// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
	D3D12_RESOURCE_BARRIER barrierDesc{};

	std::chrono::steady_clock::time_point reference_;

	// FPS�Œ菉����
	void InitFixFPS();
	// FPS�Œ�
	void UpdateFixFPS();

public:
	// �V���O���g���C���X�^���X
	static KDirectXCommon* GetInstance();

	// ���\�[�X�̏�Ԃ�ς���
	static void ResourceTransition(ID3D12Resource* resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

	// ������
	void Init();

	// �`�揀��
	void PreDraw();

	// �`��I��
	void PostDraw();

	// �R�}���h��n��
	void DeleteCommand();

	// ���
	void Destroy();

	// �w�i�̐F�ύX
	void SetBackScreenColor(float R, float G, float B, float A);

	// �f�o�C�X�擾
	ID3D12Device* GetDev() const { return dev.Get(); }

	// �R�}���h���X�g�擾
	ID3D12GraphicsCommandList* GetCmdlist() { return cmdList.Get(); }

	float fps = 0;

	// SRV,CBV,URV�p�̃f�X�N���v�^�q�[�v�擾
	KDescriptorHeap* GetSRVDescriptorHeap();

	// RTV�p�̃f�X�N���v�^�q�[�v�擾
	KRtvDescriptorHeap* GetRTVDescriptorHeap();

	// DSV�p�̃f�X�N���v�^�q�[�v�擾
	KDsvDescriptorHeap* GetDsvDescriptorHrap();

	// ���\�[�X�̏�Ԃ�ς���
	void Transition(ID3D12Resource* resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

	// �o�b�N�o�b�t�@�̐����擾
	size_t GetBackBufferCount() const;

private:
	// DXGI������
	HRESULT InitDXGIDevice();

	// �ŏI�I�ȃ����_�[�^�[�Q�b�g�̐���
	HRESULT CreateFinalRenderTarget();

	// �X���b�v�`�F�[���̐���
	HRESULT CreateSwapChain();

	// �R�}���h������
	HRESULT InitCommand();

	// �t�F���X����
	HRESULT CreateFence();

	// �[�x�֌W����
	HRESULT CreateDepthBuffer();

	// �f�o�b�O���C���[��L����
	void EnbleDebugLayer();

	// �C���t�H�L���[��L����
	void EnbleInfoQueue();

	KDirectXCommon() = default;
	~KDirectXCommon() = default;
	KDirectXCommon(const KDirectXCommon&) = default;
	const KDirectXCommon& operator=(const KDirectXCommon&) = delete;
};

