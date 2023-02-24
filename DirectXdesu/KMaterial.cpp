#include "KMaterial.h"
#include "KDirectXCommon.h"

KMaterial::~KMaterial()
{
}

void KMaterial::Initialize() {

	pipelineState = std::make_unique<KPipelineState>();

	// ���_�V�F�[�_�[��ݒ�
	if (shader->GetVSBlob() != nullptr)
	{
		pipelineState->SetVertexShader(shader->GetVSBytecode());
	}

	// �n���V�F�[�_�[��ݒ�
	if (shader->GetHSBlob() != nullptr)
	{
		pipelineState->SetHullShader(shader->GetHSBytecode());
	}

	// �h���C���V�F�[�_�[��ݒ�
	if (shader->GetDSBlob() != nullptr)
	{
		pipelineState->SetDomainShader(shader->GetDSBytecode());
	}

	// �W�I���g���V�F�[�_�[��ݒ�
	if (shader->GetGSBlob() != nullptr)
	{
		pipelineState->SetGeometryShader(shader->GetGSBytecode());
	}

	// �s�N�Z���V�F�[�_�[��ݒ�
	if (shader->GetPSBlob() != nullptr)
	{
		pipelineState->SetPixelShader(shader->GetPSBytecode());
	}

	//���_���C�A�E�g��ݒ�
	pipelineState->SetInputLayout(inputLayouts.data(), inputLayouts.size());

	//�u�����h��ݒ�
	pipelineState->SetBlend(blenddesc);

	//�[�x�t���O��ݒ�
	pipelineState->SetDepthFlag(depthFlag);

	//���[�g�V�O�l�`����ݒ�
	pipelineState->SetRootSignature(rootSignature->GetRootSignature());

	//�v���~�e�B�u�`���ݒ�
	pipelineState->SetPrimitiveType(primitiveType);

	pipelineState->SetCullMode(cullMode);

	//����
	if (!pipelineState->Create())
	{
		printf("�p�C�v���C���X�e�[�g�������ł��܂���ł���");
		assert(0);
	}
}