#pragma once
#pragma warning(push)
#pragma warning(disable: 5039)
#pragma warning(disable: 4668)
#pragma warning(disable: 5024)
#pragma warning(disable: 4820)
#pragma warning(disable: 4265)
#pragma warning(disable: 4365)
#pragma warning(disable: 5220)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 4265)
#pragma warning(disable: 5204)
#include <wrl.h>
#include <d3d12.h>
#pragma warning(pop)

class KRtvDescriptorHeap
{
private:
	//�f�o�C�X
	ID3D12Device* device;
	//�f�X�N�v���^�q�[�v
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> descriptorHeap;

	size_t maxRTV = 2048;

	size_t countRTV = 0;

	UINT incrementSize;

	char PADING2[4];


	D3D12_CPU_DESCRIPTOR_HANDLE	startCpuHandle;

public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �����_�[�^�[�Q�b�g�r���[����
	/// </summary>
	/// <param name="desc">�����_�[�^�[�Q�b�g�r���[�f�X�N</param>
	/// <param name="resource">�o�b�t�@</param>
	/// <returns>GPU�n���h��(UINT64)</returns>
	UINT64 CreateRTV(D3D12_RENDER_TARGET_VIEW_DESC& desc, ID3D12Resource* resource);

	/// <summary>
	/// �f�X�N�v���^�q�[�v���擾
	/// </summary>
	ID3D12DescriptorHeap* GetHeap();

	UINT GetIncrementSize();
};

