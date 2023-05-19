#pragma once
#include <wrl.h>
#include <d3d12.h>

class KDescriptorHeap
{
private:
	//�f�o�C�X
	Microsoft::WRL::ComPtr <ID3D12Device> device;
	//�f�X�N�v���^�q�[�v
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> descriptorHeap;
	//�f�X�N�v���^�����W
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	char PADING1[4];

	size_t maxSRV = 2048;
	size_t maxUAV = 2048;
	size_t maxCBV = 2048;

	size_t countSRV = 0;
	size_t countUAV = 0;
	size_t countCBV = 0;

	UINT incrementSize;
	char PADING2[4];


	D3D12_CPU_DESCRIPTOR_HANDLE	startCpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE	startGpuHandle;

public:

	struct DescriptorHeapViewHandle
	{
		D3D12_CPU_DESCRIPTOR_HANDLE	cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE	gpuHandle;
	};

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �V�F�[�_�[���\�[�X�r���[����
	/// </summary>
	/// <param name="desc">�V�F�[�_�[���\�[�X�r���[�f�X�N</param>
	/// <param name="resource">�o�b�t�@</param>
	/// <returns>GPU�n���h��(UINT64)</returns>
	UINT64 CreateSRV(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, ID3D12Resource* resource);

	/// <summary>
	/// �V�F�[�_�[���\�[�X�r���[�ǉ�
	/// </summary>
	DescriptorHeapViewHandle AddSRV();

	/// <summary>
	/// �A���I�[�_�[�A�N�Z�X�r���[����
	/// </summary>
	/// <param name="desc">�A���I�[�_�[�A�N�Z�X�r���[�f�X�N</param>
	/// <param name="resource">�o�b�t�@</param>
	/// <returns>GPU�n���h��(UINT64)</returns>
	UINT64 CreateUAV(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, ID3D12Resource* resource);

	/// <summary>
	/// �A���I�[�_�[�A�N�Z�X�r���[�ǉ�
	/// </summary>
	DescriptorHeapViewHandle AddUAV();

	/// <summary>
	/// �R���X�g�o�b�t�@�r���[����
	/// </summary>
	/// <param name="desc">�R���X�g�o�b�t�@�r���[�f�X�N</param>
	/// <returns>GPU�n���h��(UINT64)</returns>
	UINT64 CreateCBV(D3D12_CONSTANT_BUFFER_VIEW_DESC& desc);

	/// <summary>
	/// �R���X�g�o�b�t�@�r���[�ǉ�
	/// </summary>
	DescriptorHeapViewHandle AddCBV();

	/// <summary>
	/// �f�X�N�v���^�q�[�v���擾
	/// </summary>
	ID3D12DescriptorHeap* GetHeap();
};
