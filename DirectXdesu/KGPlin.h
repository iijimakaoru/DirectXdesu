#pragma once
#include <d3d12.h>
#include "KMaterial.h"
#include "KObject3D.h"
#include "ViewProjection.h"
#include "KMaterial.h"
#include "ViewProjection.h"
#include "KShader.h"
#include "KVertex.h"
#include "KDirectInit.h"

// ���_���C�A�E�g
static D3D12_INPUT_ELEMENT_DESC inputLayout[3] = {
	{// xyz���W
		"POSITION",										// �Z�}���e�B�b�N��
		0,												// �����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X
		DXGI_FORMAT_R32G32B32_FLOAT,					// �v�f���ƃr�b�g����\��
		0,												// ���̓X���b�g�C���f�b�N�X
		D3D12_APPEND_ALIGNED_ELEMENT,					// �f�[�^�̃I�t�Z�b�g
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		// ���̓f�[�^���
		0												// ��x�ɕ`�悷��C���X�^���X��
	},
	{// �@���x�N�g��
		"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
	{// uv���W
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
};

class KGPlin
{
public:
	KGPlin();
	KGPlin(KDirectInit dx,HRESULT result, ID3D12Device* dev,
		int width, int height);
	void GPipeline();
	void Render();
	void Buffer(HRESULT result, ID3D12Device* dev,
		int width, int height);
	void DescRipRan();
	void RootParam();
	void Sampler();
	void RootSig(HRESULT result, ID3D12Device* dev);
	void PipelineState(HRESULT result, ID3D12Device* dev);

	// �V�F�[�_�[
	Shader* shader;
	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	// �}�e���A��
	KMaterial* material;
	// 3D�I�u�W�F�N�g
	Object3D* object3d;
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
};

