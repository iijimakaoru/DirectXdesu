#include "KTexture.h"
#include "KDirectXCommon.h"
#include <string>

KTextureManager* KTextureManager::textureManager = nullptr;

void KTexture::CreateTexture(const std::string& directoryPath, const std::string& filename)
{
	// �V���O���g���̃f�o�C�X�󂯎��
	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDev();

	string filePath = directoryPath + filename;

	// ���j�R�[�h������ɕϊ�����
	wchar_t wFilePath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, wFilePath, _countof(wFilePath));

	// �e�N�X�`�����[�h
	result = LoadFromWICFile(
		wFilePath,
		WIC_FLAGS_NONE,
		&metadata, scraychImg);
	assert(SUCCEEDED(result));

	// �~�j�}�b�v�쐬
	result = GenerateMipMaps(
		scraychImg.GetImages(),
		scraychImg.GetImageCount(),
		scraychImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0, mipChain);

	if (SUCCEEDED(result)) {
		scraychImg = std::move(mipChain);
		metadata = scraychImg.GetMetadata();
	}

	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

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
		IID_PPV_ARGS(&texBuff));

	// �S�~�j�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		// �~�j�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scraychImg.GetImage(i, 0, 0);

		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch);
		assert(SUCCEEDED(result));
	}

	// SRV�p�f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // �V�F�[�_�[���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount; // �e�N�X�`���̖���
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap)); // ����
	assert(SUCCEEDED(result));

	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	device->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHeap->GetCPUDescriptorHandleForHeapStart());
}