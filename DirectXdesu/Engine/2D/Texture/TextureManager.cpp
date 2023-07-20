#include "TextureManager.h"
#include "KDirectXCommon.h"

TextureManager* TextureManager::textureManager = nullptr;
std::string TextureManager::DefaultTextureDirectoryPath = "Resource/";

void TextureManager::Init()
{
	HRESULT result;

	device = KDirectXCommon::GetInstance()->GetDev();

	// �f�X�N���v�^�����W�̐ݒ�
	descriptorRange.NumDescriptors = 1; // 1�x�̕`��Ɏg���e�N�X�`���̐�
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0; // ���W�X�^�ԍ�
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// SRV�p�f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // �V�F�[�_�[���猩����悤��
	srvHeapDesc.NumDescriptors = static_cast<UINT>(MaxSRVCount); // �e�N�X�`���̖���
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap)); // ����
	assert(SUCCEEDED(result));

	// �q�[�v�ݒ�
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
}

TextureData TextureManager::LoadTexture(const std::string& fileName)
{
	HRESULT result;

	if (texCount > 1024)
	{
		assert(0);
	}

	TextureData data{};

	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};
	DirectX::ScratchImage mipChain{};

	data.srvHeap = srvHeap;
	data.descriptorRange = descriptorRange;

	wchar_t wfilepath[256];

	MultiByteToWideChar(CP_ACP, 0, fileName.c_str(), -1, wfilepath, _countof(wfilepath));

	// �e�N�X�`�����[�h
	result = LoadFromWICFile(
		wfilepath,
		DirectX::WIC_FLAGS_NONE,
		&metadata, scratchImg);
	assert(SUCCEEDED(result));

	// �~�j�}�b�v�쐬
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) 
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = DirectX::MakeSRGB(metadata.format);

	// �e�N�X�`���o�b�t�@�̐���
	data.texBuff = CreateTexBuff(metadata, scratchImg);

	// �V�F�[�_���\�[�X�r���[�̐���
	data.gpuHandle = CreateSRV(data.texBuff.Get(), metadata);

	// �����L��
	data.width = metadata.width;

	// �c���L��
	data.height = metadata.height;

	texCount++;

	return data;
}

TextureData TextureManager::Load(const std::string& fileName)
{
	return TextureManager::GetInstance()->LoadTexture(fileName);
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateTexBuff(DirectX::TexMetadata& metadata, DirectX::ScratchImage& scratchImg)
{
	HRESULT result;

	Microsoft::WRL::ComPtr<ID3D12Resource> buff;

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// �e�N�X�`���p�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(buff.ReleaseAndGetAddressOf()));

	// �S�~�j�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++) 
	{
		// �~�j�}�b�v���x�����w�肵�ăC���[�W���擾
		const DirectX::Image* img = scratchImg.GetImage(i, 0, 0);

		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = buff->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(result));
	}

	return buff;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::CreateSRV(ID3D12Resource* texBuff, DirectX::TexMetadata& metadata)
{
	// �A�h���X�̐擪���擾
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// �n���h���̃|�C���^���炵
	cpuHandle.ptr += static_cast<UINT64>(texCount) * incrementSize;
	gpuHandle.ptr += static_cast<UINT64>(texCount) * incrementSize;

	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = static_cast<UINT>(metadata.mipLevels);

	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	device->CreateShaderResourceView(texBuff, &srvDesc, cpuHandle);

	return gpuHandle;
}

void TextureManager::Delete()
{
	delete textureManager;
}

TextureManager* TextureManager::GetInstance()
{
	static TextureManager instance;
	return&instance;
}
