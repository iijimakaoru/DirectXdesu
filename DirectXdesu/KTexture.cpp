#include "KTexture.h"

KTexture::KTexture(KDirectInit dx, KVertex vertex) {
	for (int i = 0; i < 2; i++) {
		LoadTexture(dx, i);
		GeneMipMap(dx);
		SetTextureBuff();
		GeneTextureBuff(dx, i);
		SendData(dx, i);
	}
	SetDescHeap();
	GeneDescHeap(dx);
	GetSrvHandle(dx);
	SetSRV(dx, vertex);
	for (int i = 0; i < 2; i++) {
		CreateSRV(dx, i);
	}
}

void KTexture::LoadTexture(KDirectInit dx, int i) {
	dx.result = LoadFromWICFile(
		msg[i],
		WIC_FLAGS_NONE,
		&metadata, scraychImg);
}

void KTexture::GeneMipMap(KDirectInit dx) {
	// �~�j�}�b�v�쐬
	dx.result = GenerateMipMaps(
		scraychImg.GetImages(),
		scraychImg.GetImageCount(),
		scraychImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0, mipChain
	);

	if (SUCCEEDED(dx.result)) {
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

void KTexture::GeneTextureBuff(KDirectInit dx, int i) {
	dx.result = dx.dev->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff[i]));
}

void KTexture::SendData(KDirectInit dx, int i) {
	// �S�~�j�}�b�v�ɂ���
	for (size_t j = 0; j < metadata.mipLevels; j++) {
		// �~�j�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scraychImg.GetImage(j, 0, 0);
		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		dx.result = texBuff[i]->WriteToSubresource(
			(UINT)j,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		assert(SUCCEEDED(dx.result));
	}
}

void KTexture::SetDescHeap() {
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;
}

void KTexture::GeneDescHeap(KDirectInit dx) {
	dx.result = dx.dev->CreateDescriptorHeap(
		&srvHeapDesc,
		IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(dx.result));
}

void KTexture::GetSrvHandle(KDirectInit dx) {
	// CBV,SRV,UAV�̈���̃T�C�Y���擾
	UINT descriptorSize = dx.dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	for (int i = 0; i < 2; i++) {
		// SRV�q�[�v�̐擪�n���h�����擾
		srvHandle[i] = srvHeap->GetCPUDescriptorHandleForHeapStart();
		// �n���h�����P�i�߂�
		srvHandle[i].ptr += descriptorSize * i;
	}
}

void KTexture::SetSRV(KDirectInit dx, KVertex vertex) {
	srvDesc.Format = vertex.resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = vertex.resDesc.MipLevels;
}

void KTexture::CreateSRV(KDirectInit dx, int i) {
	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	dx.dev->CreateShaderResourceView(texBuff[i], &srvDesc, srvHandle[i]);
}