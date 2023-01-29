#include "Light.h"
#include "KDirectXCommon.h"

void Light::Initialize()
{
	HRESULT result;

	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �萔�o�b�t�@�̐���
	constBuff = nullptr;
	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
		&cbHeapProp, // 
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc, // 
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	// �萔�o�b�t�@�փf�[�^�]��
	TransferConstBuffer();
}

void Light::TransferConstBuffer()
{
	HRESULT result;
	// �萔�o�b�t�@�f�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->lightv = -lightdir;
		constMap->lightcolor = lightcolor;
		constBuff->Unmap(0, nullptr);
	}
}

void Light::SetLightDir(const XMVECTOR& lightDir)
{
	// ���K�����ăZ�b�g
	lightdir = XMVector3Normalize(lightDir);
	dirty = true;
}

void Light::SetLightColor(const XMFLOAT3& lightColor)
{
	lightcolor = lightColor;
	dirty = true;
}

void Light::Update()
{
	if (dirty)
	{
		TransferConstBuffer();
		dirty = false;
	}
}

void Light::Draw(UINT rootParameterIndex)
{
	// �萔�o�b�t�@�r���[���Z�b�g
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());
}

Light* Light::Create()
{
	Light* instance = new Light();
	instance->Initialize();
	return instance;
}
