#include <stdio.h>
#include <d3dcompiler.h>
#include "KWindow.h"
#include "KDirectInit.h"
#include "KInput.h"
#include "KDepth.h"
#include "KTexture.h"
#include "KWorldTransform.h"
#include "ViewProjection.h"
//#include "KGPlin.h"
#include "KModel.h"
#include "KShader.h"
#ifdef _DEBUG
#include <iostream>
#endif
#pragma comment(lib, "d3dcompiler.Lib")
#include "Sound.h"
#include <d3dx12.h>
#include "ConstBuffer.h"
#include "PipelineSet.h"
#include "Vector4.h"

const int spriteSRVCount = 512;

struct VertexPosUV
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

struct Sprite
{
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//	���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// Z������̉�]�p
	float rotation = 0.0f;
	// ���W
	Vector3 position = { 0,0,0 };
	// ���[���h�s��
	XMMATRIX matWorld;
	// �F
	XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
	// �e�N�X�`���ԍ�
	UINT texNum = 0;
};

struct SpriteCommon
{
	// �p�C�v���C���Z�b�g
	PipelineSet pipelineSet;
	// �ˉe�s��
	XMMATRIX matProjection{};
	// �e�N�X�`���p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// �e�N�X�`�����\�[�X(�e�N�X�`���o�b�t�@)�̔z��
	ComPtr<ID3D12Resource> texBuff[spriteSRVCount];
};

PipelineSet Create3DObjectGpipeline(ID3D12Device* dev)
{
	HRESULT result;
#pragma region �V�F�[�_�[�ǂݍ��݂ƃR���p�C��
	std::unique_ptr<KShader> shader;
	shader = std::make_unique<KShader>();
	shader->BasicPSLoadCompile();
	shader->BasicVSLoadCompile();
#pragma endregion
#pragma region ���_���C�A�E�g�z��̐錾�Ɛݒ�
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
#pragma endregion
#pragma region �p�C�v���C���X�e�[�g�ݒ�ϐ��̐錾�Ɗe�퍀�ڂ̐ݒ�
	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	// �V�F�[�_�[�̐ݒ�
	gpipeline.VS.pShaderBytecode = shader->vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = shader->vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = shader->psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = shader->psBlob->GetBufferSize();
	// �T���v���}�X�N�̐ݒ�
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// ���X�^���C�U�̐ݒ�
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // �w�ʂ��J�����O
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	gpipeline.RasterizerState.DepthClipEnable = true;
	// �u�����h�X�e�[�g
	gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);
	// �}�`�̌`��ݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// ���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gpipeline.SampleDesc.Count = 1;
	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	// ����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	// �f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	// ���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParam[3] = {};
	// �萔�o�b�t�@0��
	rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[0].Descriptor.ShaderRegister = 0;
	rootParam[0].Descriptor.RegisterSpace = 0;
	rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// �e�N�X�`�����W�X�^0��
	rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
	rootParam[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// �萔�o�b�t�@1��
	rootParam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[2].Descriptor.ShaderRegister = 1;
	rootParam[2].Descriptor.RegisterSpace = 0;
	rootParam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// �e�N�X�`���T���u���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
#pragma endregion
	// �p�C�v���C���ƃ��[�g�V�O�l�`���̃Z�b�g
	PipelineSet pipelineSet;
#pragma region ���[�g�V�O�l�`���̐���
	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam;
	rootSignatureDesc.NumParameters = _countof(rootParam);
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &shader->errorBlob);
	assert(SUCCEEDED(result));
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.rootSignature));
	assert(SUCCEEDED(result));
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = pipelineSet.rootSignature.Get();
#pragma endregion
#pragma region �O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐���
	// �O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState.DepthEnable = true; // �[�x�e�X�g
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // �������݋���
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // ��������΍��i
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g

	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet.pipelineState));
	assert(SUCCEEDED(result));
#pragma endregion

	return pipelineSet;
}

PipelineSet SpriteCreateGraphicsPipeline(ID3D12Device* dev)
{
	HRESULT result;
#pragma region �V�F�[�_�[�ǂݍ��݂ƃR���p�C��
	std::unique_ptr<KShader> shader;
	shader = std::make_unique<KShader>();
	shader->SpritePSLoadCompile();
	shader->SpriteVSLoadCompile();
#pragma endregion
#pragma region ���_���C�A�E�g�z��̐錾�Ɛݒ�
	static D3D12_INPUT_ELEMENT_DESC inputLayout[2] = {
	{// xyz���W
		"POSITION",										// �Z�}���e�B�b�N��
		0,												// �����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X
		DXGI_FORMAT_R32G32B32_FLOAT,					// �v�f���ƃr�b�g����\��
		0,												// ���̓X���b�g�C���f�b�N�X
		D3D12_APPEND_ALIGNED_ELEMENT,					// �f�[�^�̃I�t�Z�b�g
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		// ���̓f�[�^���
		0												// ��x�ɕ`�悷��C���X�^���X��
	},
	{// uv���W
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
	};
#pragma endregion
#pragma region �p�C�v���C���X�e�[�g�ݒ�ϐ��̐錾�Ɗe�퍀�ڂ̐ݒ�
	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	// �V�F�[�_�[�̐ݒ�
	gpipeline.VS.pShaderBytecode = shader->vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = shader->vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = shader->psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = shader->psBlob->GetBufferSize();
	// �T���v���}�X�N�̐ݒ�
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// ���X�^���C�U�̐ݒ�
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �w�ʂ��J�����O���Ȃ�
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	gpipeline.RasterizerState.DepthClipEnable = true;
	// �u�����h�X�e�[�g
	gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);
	// �}�`�̌`��ݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// ���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gpipeline.SampleDesc.Count = 1;
	// �[�x�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthEnable = false;
	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	// ����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	// �f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	// ���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParam[3] = {};
	// �萔�o�b�t�@0��
	rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[0].Descriptor.ShaderRegister = 0;
	rootParam[0].Descriptor.RegisterSpace = 0;
	rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// �e�N�X�`�����W�X�^0��
	rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
	rootParam[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// �萔�o�b�t�@1��
	rootParam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[2].Descriptor.ShaderRegister = 1;
	rootParam[2].Descriptor.RegisterSpace = 0;
	rootParam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// �e�N�X�`���T���u���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
#pragma endregion
	// �p�C�v���C���ƃ��[�g�V�O�l�`���̃Z�b�g
	PipelineSet pipelineSet;
#pragma region ���[�g�V�O�l�`���̐���
	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam;
	rootSignatureDesc.NumParameters = _countof(rootParam);
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &shader->errorBlob);
	assert(SUCCEEDED(result));
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.rootSignature));
	assert(SUCCEEDED(result));
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = pipelineSet.rootSignature.Get();
#pragma endregion
#pragma region �O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐���
	// �O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState.DepthEnable = true; // �[�x�e�X�g
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // �������݋���
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // ��������΍��i
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g

	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet.pipelineState));
	assert(SUCCEEDED(result));
#pragma endregion

	return pipelineSet;
}

Sprite SpriteCreate(ID3D12Device* dev, int window_width, int window_height)
{
	HRESULT result = S_FALSE;
	// �V�����X�v���C�g�����
	Sprite sprite{};
	// ���_�f�[�^
	VertexPosUV vertices[] =
	{
		// x	  y		 z		  u	   v
		{{  0.0f,100.0f,  0.0f},{0.0f,1.0f}}, // ���� 
		{{  0.0f,  0.0f,  0.0f},{0.0f,0.0f}}, // ����
		{{100.0f,100.0f,  0.0f},{1.0f,1.0f}}, // �E��
		{{100.0f,  0.0f,  0.0f},{1.0f,0.0f}}, // �E��
	};
	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = static_cast<UINT>(sizeof(vertices));
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// ���_�o�b�t�@����
	result = dev->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&sprite.vertBuff)
	);
	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUV* vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);
	// ���_�o�b�t�@�r���[�̍쐬
	sprite.vbView.BufferLocation = sprite.vertBuff->GetGPUVirtualAddress();
	sprite.vbView.SizeInBytes = sizeof(vertices);
	sprite.vbView.StrideInBytes = sizeof(vertices[0]);
	// �萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&sprite.constBuff)
	);
	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = sprite.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1); // �F�w��
	// ���s���e�s��
	constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);
	sprite.constBuff->Unmap(0, nullptr);

	return sprite;
}

void SpriteCommonBeginDraw(ID3D12GraphicsCommandList* cmdList, const SpriteCommon& spriteCommon)
{
	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(spriteCommon.pipelineSet.pipelineState.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(spriteCommon.pipelineSet.rootSignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	// �e�N�X�`���p�f�X�N���v�^�q�[�v�̐ݒ�
	ID3D12DescriptorHeap* ppHeaps[] = { spriteCommon.descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void SpriteDraw(const Sprite& sprite, ID3D12GraphicsCommandList* cmdList,
	const SpriteCommon& spriteCommon, ID3D12Device* dev)
{
	// ���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &sprite.vbView);
	// �萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, sprite.constBuff->GetGPUVirtualAddress());
	// �V�F�[�_�[���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(
		1, 
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			spriteCommon.descHeap->GetGPUDescriptorHandleForHeapStart(),
			sprite.texNum,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	// �|���S���̕`��
	cmdList->DrawInstanced(4, 1, 0, 0);
}

SpriteCommon SpriteCommonCreate(ID3D12Device* dev, int window_width, int window_height)
{
	HRESULT result = S_FALSE;

	SpriteCommon spriteCommon{};

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = spriteSRVCount;

	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&spriteCommon.descHeap));

	spriteCommon.pipelineSet = SpriteCreateGraphicsPipeline(dev);

	spriteCommon.matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window_width, (float)window_height, 0.0f, 0.0f, 1.0f);

	return spriteCommon;
}

void SpriteUpdate(Sprite& sprite, const SpriteCommon& spriteCommon)
{
	// ���[���h�s��
	sprite.matWorld = XMMatrixIdentity();
	// Z����]
	sprite.matWorld *= XMMatrixRotationZ(XMConvertToRadians(sprite.rotation));
	// ���s�ړ�
	sprite.matWorld *= XMMatrixTranslation(sprite.position.x, sprite.position.y, sprite.position.z);
	// �萔�o�b�t�@�̓]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = sprite.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = sprite.matWorld * spriteCommon.matProjection;
	constMap->color = sprite.color;
	sprite.constBuff->Unmap(0, nullptr);
}

HRESULT SpriteCommonLoadTexture(SpriteCommon& spriteCommon,
	UINT texnumber, const wchar_t* filename, ID3D12Device* dev)
{
	assert(texnumber <= spriteSRVCount - 1);

	HRESULT result;
#pragma region WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scraychImg{};
	// �摜�ǂݍ���
	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scraychImg);

	ScratchImage mipChain{};
	// �~�j�}�b�v�쐬
	result = GenerateMipMaps(
		scraychImg.GetImages(),
		scraychImg.GetImageCount(),
		scraychImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0, mipChain);

	if (SUCCEEDED(result)) {
		scraychImg = std::move(mipChain);
		metadata = scraychImg.GetMetadata();
	}

	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);
#pragma endregion
#pragma region ���\�[�X�ݒ�
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;
	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
#pragma endregion
#pragma region �e�N�X�`���o�b�t�@�̐���
	spriteCommon.texBuff[texnumber] = nullptr;
	// �e�N�X�`���o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteCommon.texBuff[texnumber]));
#pragma endregion
#pragma region �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	// �S�~�j�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		// �~�j�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scraychImg.GetImage(i, 0, 0);
		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = spriteCommon.texBuff[texnumber]->WriteToSubresource(
			(UINT)i,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch);
		assert(SUCCEEDED(result));
	}
#pragma endregion

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = spriteSRVCount;

	ID3D12DescriptorHeap* srvHeap = nullptr;
	result = dev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	// SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	// �n���h���̎�����ɃV�F�[�_�[���\�[�X�r���[�쐬
	dev->CreateShaderResourceView(spriteCommon.texBuff[texnumber].Get(), &srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(spriteCommon.descHeap->GetCPUDescriptorHandleForHeapStart(),texnumber,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	return S_OK;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
	// �f�o�b�O���C���[���I����
	ID3D12Debug1* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(true);
	}
#endif
	HRESULT result;

#pragma region �E�B���h�E
	KWindow win;
#pragma endregion
#pragma region DirectX������
	KDirectInit dx(win);
	//#ifdef _DEBUG
	//	ID3D12InfoQueue* infoQueue;
	//	if (SUCCEEDED(dx.dev->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
	//		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
	//		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	//		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
	//		infoQueue->Release();
	//	}
	//#endif // _DEBUG
	KInput input(win.window, win.handle);
#pragma endregion
#pragma region �`�揉����
#pragma region �[�x�o�b�t�@
	KDepth depth(dx.SetDev().Get(), win);
#pragma endregion
	// ����
	float speed = 1.0f;
#pragma region ���f��
	KModel triangle = Triangle();
	triangle.CreateModel(dx.SetDev().Get());
	KModel cube = Cube();
	cube.CreateModel(dx.SetDev().Get());
	KModel line = Line();
	line.CreateModel(dx.SetDev().Get());
#pragma endregion
#pragma region �e�N�X�`��������
	const wchar_t* msg = L"Resources/mario.jpg";
	const wchar_t* msg2 = L"Resources/iijan.jpg";
	const wchar_t* msg3 = L"Resources/haikei.jpg";
	const wchar_t* msg4 = L"Resources/kitanai.jpg";
	KTexture texture(dx.SetDev().Get(), msg, msg3);
	KTexture texture2(dx.SetDev().Get(), msg2, msg4);
#pragma endregion

#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�
	// 3D�I�u�W�F�N�g�p�p�C�v���C������
	PipelineSet object3dPipelineSet = Create3DObjectGpipeline(dx.SetDev().Get());
	// �X�v���C�g�p�p�C�v���C������
	PipelineSet spritePipelineSet = SpriteCreateGraphicsPipeline(dx.SetDev().Get());
#pragma region 3D�I�u�W�F�N�g������
	const int ObjectNum = 2;
	const int LineNum = 6;
	// 3D�I�u�W�F�N�g
	KWorldTransform object3d[ObjectNum];
	//KWorldTransform lineObject[LineNum];
	for (int i = 0; i < ObjectNum; i++) {
		object3d[i].Initialize(*dx.SetDev().Get());
		if (i > 0) {
			object3d[i].material->colorR = object3d[i].material->colorG = object3d[i].material->colorB = 1.0f;
		}
	}
	object3d[0].SetModel(&cube);
	object3d[0].SetTexture(&texture);
	object3d[1].SetModel(&cube);
	object3d[1].SetTexture(&texture2);
	/*for (int i = 0; i < LineNum; i++) {
		lineObject[i].Initialize(*dx.dev);
		lineObject[i].SetModel(&line);
	}
	lineObject[1].transform.pos.x = lineObject[0].transform.pos.x + 20;
	lineObject[2].transform.pos.x = lineObject[0].transform.pos.x - 20;
	lineObject[3].transform.rot.y = XMConvertToRadians(90);
	lineObject[4].transform.rot.y = XMConvertToRadians(90);
	lineObject[4].transform.pos.z = lineObject[3].transform.pos.z + 20;
	lineObject[5].transform.rot.y = XMConvertToRadians(90);
	lineObject[5].transform.pos.z = lineObject[3].transform.pos.z - 20;*/
#pragma endregion
#pragma region �r���[
	// �r���[�v���W�F�N�V����
	ViewProjection* viewProjection;
	viewProjection = new ViewProjection(win.window_width, win.window_height);
#pragma endregion

#pragma endregion
	Vector3 center = { 0,0,1 };

	float rSpeed = -0.02f;
	float gSpeed = 0.02f;
	float bSpeed = -0.02f;
	float aSpeed = -0.02f;

	std::unique_ptr<Sound> sound;
	sound = std::make_unique<Sound>();
	sound->Init();

	SoundData soundData1 = sound->SoundLoadWave("Sound/fanfare.wav");

	SpriteCommon spriteCommon;
	spriteCommon = SpriteCommonCreate(dx.SetDev().Get(), win.window_width, win.window_height);

	SpriteCommonLoadTexture(spriteCommon, 0, L"Resources/haikei.jpg", dx.SetDev().Get());
	SpriteCommonLoadTexture(spriteCommon, 1, L"Resources/mario.jpg", dx.SetDev().Get());

	Sprite sprite[2];
	for (int i = 0; i < _countof(sprite); i++)
	{
		sprite[i] = SpriteCreate(dx.SetDev().Get(), win.window_width, win.window_height);
	}
	sprite[0].texNum = 0;
	sprite[1].texNum = 1;
	sprite[1].rotation = 45;
	sprite[1].position = { 1280 / 2, 720 / 2,0 };
	//#pragma region �X�v���C�g
	//	std::unique_ptr<Sprite> sprite;
	//	sprite = std::make_unique<Sprite>();
	//
	//	Sprite::SpriteCommon spriteCommon;
	//	spriteCommon = sprite->SpriteCommonCreate(dx.SetDev().Get());
	//#pragma endregion
	//	Sprite::SpriteBuff spriteBuff;
	//	spriteBuff = sprite->SpriteCreate(dx.SetDev().Get(), win.window_width, win.window_height);
#pragma endregion

	// �E�B���h�E�\��
	// �Q�[�����[�v
	while (true)
	{
#pragma region �E�B���h�E���b�Z�[�W
		if (win.breakFlag || input.IsPush(DIK_ESCAPE)) {
			sound->GetxAudio().Reset();
			sound->SoundUnLoad(&soundData1);
			break;
		}
#pragma endregion

		// �X�V
#pragma region DirectX���t���[������
		for (int i = 0; i < _countof(sprite); i++)
		{
			SpriteUpdate(sprite[i], spriteCommon);
		}
#pragma region �E�B���h�E�A�b�v�f�[�g
		win.Update();
#pragma endregion

#pragma region input�A�b�v�f�[�g
		input.Update();
#pragma endregion

#pragma region �L�[�{�[�h����
		// �w�i�F�ς�
		if (input.IsTriger(DIK_SPACE)) {
			sound->SoundPlayWave(sound->GetxAudio().Get(), soundData1);
		}
		// �摜�F�ς�
		if (object3d[0].material->colorR <= 0 || object3d[0].material->colorR >= 1) {
			rSpeed *= -1;
		}
		if (object3d[0].material->colorG <= 0 || object3d[0].material->colorG >= 1) {
			gSpeed *= -1;
		}
		if (object3d[0].material->colorB <= 0 || object3d[0].material->colorB >= 1) {
			bSpeed *= -1;
		}
		if (object3d[0].material->colorA <= 0 || object3d[0].material->colorA >= 1) {
			aSpeed *= -1;
		}
		object3d[0].material->colorR += rSpeed;
		object3d[0].material->colorG += gSpeed;
		object3d[0].material->colorB += bSpeed;
		if (input.IsPush(DIK_X)) {
			object3d[0].material->colorA += aSpeed;
		}
		// �}�`�c��]
		if (input.IsPush(DIK_C) ||
			input.IsPush(DIK_V)) {
			if (input.IsPush(DIK_C)) {
				object3d[0].transform.rot.z += 0.1f;
			}

			if (input.IsPush(DIK_V)) {
				object3d[0].transform.rot.z -= 0.1f;
			}
		}
		////�J�����ړ�
		if (input.IsPush(DIK_D) || input.IsPush(DIK_A) ||
			input.IsPush(DIK_W) || input.IsPush(DIK_S)) {
			if (input.IsPush(DIK_D)) {
				viewProjection->angleX += XMConvertToRadians(1.0f);
			}
			else if (input.IsPush(DIK_A)) {
				viewProjection->angleX -= XMConvertToRadians(1.0f);
			}
			if (input.IsPush(DIK_W)) {
				viewProjection->angleY -= XMConvertToRadians(1.0f);
			}
			else if (input.IsPush(DIK_S)) {
				viewProjection->angleY += XMConvertToRadians(1.0f);
			}
			// angle���W�A��y����]
			viewProjection->eye.x = viewProjection->lenZ * sinf(viewProjection->angleX);
			viewProjection->eye.y = viewProjection->lenZ * sinf(viewProjection->angleY);
			viewProjection->eye.z = viewProjection->lenZ * cosf(viewProjection->angleX) * cosf(viewProjection->angleY);
		}
		// ����]
		if (input.IsPush(DIK_RIGHT) ||
			input.IsPush(DIK_LEFT)) {
			if (input.IsPush(DIK_RIGHT)) {
				object3d[0].transform.rot.y -= 0.1f;
			}
			if (input.IsPush(DIK_LEFT)) {
				object3d[0].transform.rot.y += 0.1f;
			}
		}
		//// �ړ�
		//if (input.IsPush(DIK_UP) ||
		//	input.IsPush(DIK_DOWN)) {
		//	if (input.IsPush(DIK_UP)) {
		//		speed = 1;
		//	}
		//	if (input.IsPush(DIK_DOWN)) {
		//		speed = -1;
		//	}
		//}
		//else {
		//	speed = 0;
		//}
		// �O�x�N�g��
		object3d[0].rotResult.x = sin(object3d[0].transform.rot.y) * center.z;
		object3d[0].rotResult.z = cos(object3d[0].transform.rot.y) * center.z;
		// �ړ�
		/*object3d[0]->transform.pos.x += (speed)*object3d[0]->rotResult.x;
		object3d[0]->transform.pos.z += (speed)*object3d[0]->rotResult.z;*/
		object3d[0].transform.pos.x += (input.IsPush(DIK_RIGHT) - input.IsPush(DIK_LEFT)) * speed;
		object3d[0].transform.pos.z += (input.IsPush(DIK_UP) - input.IsPush(DIK_DOWN)) * speed;

		object3d[1].transform.pos.x = object3d[0].transform.pos.x + 15;
		object3d[1].transform.pos.z = object3d[0].transform.pos.z;
		object3d[1].transform.pos.y = object3d[0].transform.pos.y + 10;

		object3d[1].transform.rot = object3d[0].transform.rot;
#pragma endregion

#pragma region �摜�F�A�b�v�f�[�g
		//material->Update();
#pragma endregion

#pragma region �r���[�̃A�b�v�f�[�g
		viewProjection->Update(win.window_width, win.window_height);
#pragma endregion

#pragma region 3D�I�u�W�F�N�g�̃A�b�v�f�[�g
		for (int i = 0; i < ObjectNum; i++) {
			object3d[i].Update(viewProjection->matView, viewProjection->matProjection);
		}
		/*for (int i = 0; i < LineNum; i++) {
			lineObject[i].Update(viewProjection->matView, viewProjection->matProjection);
		}*/
#pragma endregion

#pragma endregion

		// �`��
#pragma region ���\�[�X�o���A
		// �o�b�N�o�b�t�@�̔ԍ����擾
		UINT bbIndex = dx.SetSChain()->GetCurrentBackBufferIndex();
		// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = dx.SetBackBuffers()[bbIndex].Get();
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		dx.SetCmdlist()->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region �`���
		// 2. �`���̕ύX
		// �����_�[�^�[�Q�[�g�r���[�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx.SetRtvHeap()->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * dx.SetDev()->GetDescriptorHandleIncrementSize(dx.SetRtvHeapDesc().Type);
		// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depth.dsvHeap->GetCPUDescriptorHandleForHeapStart();
		dx.SetCmdlist()->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
#pragma endregion

#pragma region ��ʃN���A
		// 3. ��ʃN���A
		FLOAT clearColor[] = { dx.bRed,dx.bGreen,dx.bBule,0.0f };
		dx.SetCmdlist()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		dx.SetCmdlist()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion

#pragma region �`��
		// �`��R�}���h��������
#pragma region �r���[�|�[�g�ݒ�R�}���h
		// �r���[�|�[�g�ݒ�R�}���h
		D3D12_VIEWPORT viewport{};
		viewport.Width = win.window_width;   // ����
		viewport.Height = win.window_height; // �c��
		viewport.TopLeftX = 0;                 // ����x
		viewport.TopLeftY = 0;				   // ����y
		viewport.MinDepth = 0.0f;			   // �ŏ��[�x
		viewport.MaxDepth = 1.0f;			   // �ő�[�x
		// �r���[�|�[�g�ݒ�R�}���h���R�}���h���X�g�ɐς�
		dx.SetCmdlist()->RSSetViewports(1, &viewport);
#pragma endregion
#pragma region �V�U�[��`�ݒ�
		// �V�U�[��`
		D3D12_RECT scissorRect{};
		scissorRect.left = 0;									// �؂蔲�����W��
		scissorRect.right = scissorRect.left + win.window_width;	// �؂蔲�����W�E
		scissorRect.top = 0;									// �؂蔲�����W��
		scissorRect.bottom = scissorRect.top + win.window_height;	// �؂蔲�����W��
		// �V�U�[��`�ݒ�R�}���h���R�}���h���X�g�ɐς�
		dx.SetCmdlist()->RSSetScissorRects(1, &scissorRect);
#pragma endregion
#pragma region �p�C�v���C���X�e�[�g�ݒ�
		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		dx.SetCmdlist()->SetPipelineState(object3dPipelineSet.pipelineState.Get());
		dx.SetCmdlist()->SetGraphicsRootSignature(object3dPipelineSet.rootSignature.Get());
#pragma endregion
#pragma region �v���~�e�B�u�`��
		// �v���~�e�B�u�`��̐ݒ�R�}���h
		dx.SetCmdlist()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion

#pragma region �`��R�}���h
		// �`��R�}���h
		for (int i = 0; i < ObjectNum; i++) {
			if (!input.IsPush(DIK_SPACE))
			{
				object3d[i].Draw(dx.SetCmdlist());
			}
			else
			{
				object3d[i].SecoundDraw(dx.SetCmdlist());
			}
		}
		/*for (int i = 0; i < LineNum; i++) {
			lineObject[i].Draw(dx.cmdList);
		}*/
		// �X�v���C�g�`��
		SpriteCommonBeginDraw(dx.SetCmdlist(), spriteCommon);
		for (int i = 0; i < _countof(sprite); i++)
		{
			SpriteDraw(sprite[i], dx.SetCmdlist(), spriteCommon, dx.SetDev().Get());
		}
		// �`��R�}���h�����܂�
#pragma endregion
#pragma endregion
#pragma region ���\�[�X�o���A��߂�
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		dx.SetCmdlist()->ResourceBarrier(1, &barrierDesc);
#pragma endregion
#pragma region �R�}���h�̃t���b�V��
		dx.CmdFlash();
#pragma endregion
#pragma region �R�}���h�����҂�
		dx.CmdClear();
#pragma endregion
	}

	return 0;
}