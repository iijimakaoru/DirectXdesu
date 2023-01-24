#include "ConstBuffer.h"
#include "KDirectXCommon.h"

ConstBuff::ConstBuff()
{
	// �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// ���\�[�X�ݒ�
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;

	// ����
	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
		&heapProp, // 
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc, // 
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&material)
	);
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̃}�b�s���O
	result = material->Map(0, nullptr, (void**)&mapMaterial); // �}�b�s���O
	assert(SUCCEEDED(result));

	ChangeColor(Vector4(1, 1, 1, 1));

	resourceDesc.Width = (sizeof(ConstBufferTimeMaterial) + 0xff) & ~0xff;

	// ����
	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
		&heapProp, // 
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc, // 
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&timeMaterial)
	);
	assert(SUCCEEDED(result));

	result = timeMaterial->Map(0, nullptr, (void**)&timeMapMaterial);
	assert(SUCCEEDED(result));

	// GPU�̃������Ƀf�[�^�]��
	timeMapMaterial->time = 0;
}

ConstBuff::~ConstBuff()
{
	material->Unmap(0, nullptr);
	timeMaterial->Unmap(0, nullptr);
}

void ConstBuff::ChangeColor(const Vector4& color)
{
	// GPU�̃������փf�[�^�]��
	mapMaterial->color = color;
}

void ConstBuff::TimeUpdate()
{
	timeMapMaterial->time += 0.1f;
}

void ConstBuff::Update()
{
	// �萔�o�b�t�@�r���[�̐ݒ�R�}���h
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(1, timeMaterial->GetGPUVirtualAddress());
}

void ConstBuff::SetBufferView()
{
	// �萔�o�b�t�@�r���[�̐ݒ�R�}���h
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(0, material->GetGPUVirtualAddress());
}
