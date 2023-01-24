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
		IID_PPV_ARGS(&constBuff)
	);
}
