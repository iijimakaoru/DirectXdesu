#include "Light.h"

ID3D12Device* Light::device = nullptr;

void Light::Staticlnitialize(ID3D12Device* device)
{
	// �ď�����
	assert(!Light::device);
	// nullptr�`�F�b�N
	assert(device);
	// �ÓI�����o�ϐ��̃Z�b�g
	Light::device = device;
}

void Light::Initialize()
{
	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};

	// �萔�o�b�t�@�̐���
}
