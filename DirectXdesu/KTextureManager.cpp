#include "KTextureManager.h"
#include "KFunctionUtility.h"

KTextureManager* KTextureManager::textureManager = nullptr;
std::vector<std::string>KTextureManager::filePaths;
std::unordered_map<std::string, std::unique_ptr<KTextureData>>KTextureManager::textureDatas;

uint32_t KTextureManager::LoadTexture(const std::string& path)
{
	if (nextTexture > 2024)
	{
		assert(0);
	}

	//���ǂݍ��񂾂��Ƃ�����t�@�C���͂��̂܂ܕԂ�
	auto itr = find_if(textureDatas.begin(), textureDatas.end(), [&](std::pair<const std::string, std::unique_ptr<KTextureData, std::default_delete<KTextureData>>>& p)
		{
			return p.second->path == path;
		});

	if (itr == textureDatas.end())
	{
		std::unique_ptr<KTextureData> data;

		data.reset(FromTextureData(path));
		data->textureHandle = nextTexture;
		data->path = path;

		textureDatas[path] = std::move(data);
		filePaths[nextTexture] = path;
		uint32_t handl = nextTexture;

		nextTexture++;

		return handl;
	}
	else
	{

		uint32_t modelHandle = textureDatas[path]->textureHandle;

		return modelHandle;
	}
}

void KTextureManager::Initialize()
{
	dx = KDirectXCommon::GetInstance();

	// �q�[�v�ݒ�
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	filePaths.resize(static_cast<size_t>(2024));
}

KTextureManager* KTextureManager::GetInstance()
{
	if (textureManager == nullptr)
	{
		textureManager = new KTextureManager();
	}

	return textureManager;
}

void KTextureManager::Destroy()
{
	delete textureManager;
}

uint32_t KTextureManager::Load(const std::string& path)
{
	return KTextureManager::GetInstance()->LoadTexture(path);
}

KTextureData* KTextureManager::GetTextureData(uint32_t handle)
{
	return textureDatas[filePaths[handle]].get();
}

Microsoft::WRL::ComPtr<ID3D12Resource> KTextureManager::CreateTexBuff(DirectX::TexMetadata& metadata, DirectX::ScratchImage& scratchImg)
{
	Microsoft::WRL::ComPtr<ID3D12Resource> result;
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;//��
	textureResourceDesc.Height = (UINT)metadata.height;//����
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// �e�N�X�`���o�b�t�@�̐���
	HRESULT hr = dx->GetDev()->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(result.ReleaseAndGetAddressOf()));

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		// �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const DirectX::Image* img = scratchImg.GetImage(i, 0, 0);
		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		hr = result->WriteToSubresource(
			(UINT)i,
			nullptr,              // �S�̈�փR�s�[
			img->pixels,          // ���f�[�^�A�h���X
			(UINT)img->rowPitch,  // 1���C���T�C�Y
			(UINT)img->slicePitch // 1���T�C�Y
		);
		assert(SUCCEEDED(hr));
	}

	return result;
}

D3D12_GPU_DESCRIPTOR_HANDLE KTextureManager::CreateShaderResourceView(ID3D12Resource* texBuff, DirectX::TexMetadata& metadata)
{
	// �V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = static_cast<UINT>(metadata.mipLevels);

	D3D12_GPU_DESCRIPTOR_HANDLE result{};

	result.ptr = dx->GetSRVDescriptorHeap()->CreateSRV(srvDesc, texBuff);

	return result;
}

void KTextureManager::LoadFile(const std::string& path, DirectX::TexMetadata& metadata, DirectX::ScratchImage& scratchImg)
{
	wchar_t wfilepath[256];
	HRESULT result = 0;

	switch (GetFileType(path))
	{
	case WIC:
		MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, wfilepath, _countof(wfilepath));
		// WIC�e�N�X�`���̃��[�h
		result = LoadFromWICFile(
			wfilepath,
			DirectX::WIC_FLAGS_NONE,
			&metadata, scratchImg);
		assert(SUCCEEDED(result));
		break;

	case TGA:
		MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, wfilepath, _countof(wfilepath));
		// TGA�e�N�X�`���̃��[�h
		result = LoadFromTGAFile(
			wfilepath,
			&metadata, scratchImg);
		assert(SUCCEEDED(result));
		break;

	case PSD:
	{
		std::string texPath = KFunctionUtility::ReplaceExtension(path, "tga");

		MultiByteToWideChar(CP_ACP, 0, texPath.c_str(), -1, wfilepath, _countof(wfilepath));

		// TGA�e�N�X�`���̃��[�h
		result = LoadFromTGAFile(
			wfilepath,
			&metadata, scratchImg);
		assert(SUCCEEDED(result));
		break;
	}
	case ETC:
		assert(0);
		break;
	default:
		assert(0);
		break;
	}
}

KTextureData* KTextureManager::FromTextureData(const std::string& path)
{
	KTextureData* result = new KTextureData();

	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};
	DirectX::ScratchImage mipChain{};

	result->srvHeap = dx->GetSRVDescriptorHeap()->GetHeap();

	LoadFile(path, metadata, scratchImg);

	//�~�b�v�}�b�v����
	HRESULT hr = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(hr))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	//�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = DirectX::MakeSRGB(metadata.format);

	result->texBuff = CreateTexBuff(metadata, scratchImg);

	result->gpuHandle = CreateShaderResourceView(result->texBuff.Get(), metadata);

	result->width = metadata.width;
	result->height = metadata.height;

	return result;
}

KTextureManager::ImgFileType KTextureManager::GetFileType(const std::string& path)
{
	std::string extend = KFunctionUtility::FileExtension(path);
	if (extend == "png" ||
		extend == "bmp" ||
		extend == "gif" ||
		extend == "tiff" ||
		extend == "jpeg" ||
		extend == "jpg")
	{
		return WIC;
	}
	else if (extend == "tga")
	{
		return TGA;
	}
	else if (extend == "psd")
	{
		return PSD;
	}
	else
	{
		return ETC;
	}
}
