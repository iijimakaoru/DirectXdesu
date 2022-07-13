#include "KTexture.h"

KTexture::KTexture(ID3D12Device* dev,KVertex vertex) {
	LoadTexture();
	GeneMipMap();
	SetTextureBuff();
	GeneTextureBuff(dev);
	SendData();
	SetDescHeap();
	GeneDescHeap(dev);
	GetSrvHandle();
	SetSRV(vertex);
	CreateSRV(dev);
}

void KTexture::LoadTexture() {
	result = LoadFromWICFile(
		L"Resources/haikei.jpg",
		WIC_FLAGS_NONE,
		&metadata, scraychImg);
}

void KTexture::GeneMipMap() {
	// �~�j�}�b�v�쐬
	result = GenerateMipMaps(
		scraychImg.GetImages(),
		scraychImg.GetImageCount(),
		scraychImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0, mipChain
	);

	if (SUCCEEDED(result)) {
		scraychImg = std::move(mipChain);
		metadata = scraychImg.GetMetadata();
	}

	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);
}

void KTexture::SetTextureBuff() {
	// �q�[�v�ݒ�
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	// ���\�[�X�ݒ�
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;
}

void KTexture::GeneTextureBuff(ID3D12Device* dev) {
	result = dev->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);
}

void KTexture::SendData() {
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
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(result));
	}
}

void KTexture::SetDescHeap() {
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;
}

void KTexture::GeneDescHeap(ID3D12Device* dev) {
	result = dev->CreateDescriptorHeap(
		&srvHeapDesc,
		IID_PPV_ARGS(&srvHeap)
	);
	assert(SUCCEEDED(result));
}

void KTexture::GetSrvHandle() {
	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
}

void KTexture::SetSRV(KVertex vertex) {
	srvDesc.Format = vertex.resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = vertex.resDesc.MipLevels;
}

void KTexture::CreateSRV(ID3D12Device* dev) {
	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	dev->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);
}