#pragma once
#include <d3d12.h>
#include "KMaterial.h"
#include "KWorldTransform.h"
#include "ViewProjection.h"
#include "KMaterial.h"
#include "ViewProjection.h"
#include "KShader.h"
#include "KVertex.h"
#include "KDirectInit.h"

class KGPlin
{
public:
	KGPlin();
	KGPlin(ID3D12Device* dev, int width, int height, KVertex vertex);
	void GPipeline(KVertex vertex);
	void Render();
	void Buffer(ID3D12Device* dev,
		int width, int height);
	void DescRipRan();
	void RootParam();
	void Sampler();
	void RootSig(ID3D12Device* dev);
	void PipelineState(ID3D12Device* dev);

	// �V�F�[�_�[�N���X�Ăэ���
	/*KPixelShader* pxShader;
	KVertexShader* vtShader;*/
	KShader* shader;
	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	// �}�e���A��
	KMaterial* material;
	// 3D�I�u�W�F�N�g
	KWorldTransform* object3d;
	// �r���[�v���W�F�N�V����
	ViewProjection* viewProjection;
	// �f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	// ���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParam[3] = {};
	// �e�N�X�`���T���u���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	// ���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;
	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	// ���[�g�V�O�l�`���̃V���A���C�Y
	ID3DBlob* rootSigBlob = nullptr;
	// �p�C�v���C���X�e�[�g
	ID3D12PipelineState* pipelineState = nullptr;

	HRESULT result;
};

