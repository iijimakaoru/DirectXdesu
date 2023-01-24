#include "KGPlin.h"

void KGPlin::SetShader(KShader shader)
{
	// Vertex
	

	// 
}

void KGPlin::SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY shaderVisibility)
{
}

void KGPlin::SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, D3D12_DESCRIPTOR_RANGE pDescripterRange, UINT numDescripterRanges, D3D12_SHADER_VISIBILITY shaderVisibility)
{
}

void KGPlin::SetRootSignature(UINT rootParamNum)
{
}

void KGPlin::SetScreenRootSignature()
{
}

void KGPlin::Blending(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, const int mord)
{
}

KGPlin::KGPlin()
{
}

KGPlin::KGPlin(KShader shader)
{
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
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
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{// uv���W
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};

	Init(shader, inputLayout, _countof(inputLayout));
}

KGPlin::KGPlin(D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSize, KShader shader)
{
	HRESULT result;

	D3D12_DESCRIPTOR_RANGE range = {};
	range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	range.BaseShaderRegister = 0;
	range.NumDescriptors = 1;

	D3D12_ROOT_PARAMETER rp[2] = {};
	rp[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rp[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rp[0].DescriptorTable.pDescriptorRanges = &range;
	rp[0].DescriptorTable.NumDescriptorRanges = 1;

	SetRootParam(rp[1], D3D12_ROOT_PARAMETER_TYPE_CBV, 2, 0);

	D3D12_ROOT_SIGNATURE_DESC rsDesc = {};
	rsDesc.NumParameters = _countof(rp);
	rsDesc.NumStaticSamplers = 1;

	// �e�N�X�`���T���v���[
	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.MinLOD = 0.0f;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	rsDesc.pStaticSamplers = &sampler;
	rsDesc.NumStaticSamplers = 1;
	rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ComPtr<ID3D10Blob> rsBlob;
	ComPtr<ID3D10Blob> errBlob;

	result = D3D12SerializeRootSignature(
		&rsDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		rsBlob.ReleaseAndGetAddressOf(),
		errBlob.ReleaseAndGetAddressOf()
	);

	result = KDirectXCommon::GetInstance()->GetDev()->CreateRootSignature(
		0,
		rsBlob->GetBufferPointer(),
		rsBlob->GetBufferSize(),
		IID_PPV_ARGS(piplineSet.rootSignature.ReleaseAndGetAddressOf())
	);

	SetShader(shader);
	piplineDesc.DepthStencilState.DepthEnable = false;
	piplineDesc.DepthStencilState.StencilEnable = false;

	D3D12_INPUT_ELEMENT_DESC layout[2] =
	{
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
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};

	piplineDesc.InputLayout.pInputElementDescs = layout;
	piplineDesc.InputLayout.NumElements = _countof(layout);

	// �u�����f�B���O
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = piplineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	Blending(blenddesc);

	piplineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	piplineDesc.NumRenderTargets = 1;
	piplineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	//// ���W�X�^�[
	// �ݒ�
	piplineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �w�ʂ��J�����O���Ȃ�
	piplineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	piplineDesc.RasterizerState.DepthClipEnable = true;

	piplineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	piplineDesc.SampleDesc.Count = 1;
	piplineDesc.SampleDesc.Quality = 0;
	piplineDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	piplineDesc.pRootSignature = piplineSet.rootSignature.Get();
	result = KDirectXCommon::GetInstance()->GetDev()->CreateGraphicsPipelineState(
		&piplineDesc,
		IID_PPV_ARGS(piplineSet.pipelineState.ReleaseAndGetAddressOf())
	);
}

KGPlin::KGPlin(KShader shader, D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSize, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, D3D12_FILL_MODE fillmord)
{
	Init(shader, inputLayout, inputLayoutSize, topologyType, fillmord);
}

void KGPlin::Init(KShader shader, D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT inputLayoutSize, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, D3D12_FILL_MODE fillmord, D3D12_CULL_MODE cullmord, bool isDeep)
{
	HRESULT result;

	// �V�F�[�_�[�ݒ�
	SetShader(shader);

	// �T���v���}�X�N�̐ݒ�
	piplineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//// ���W�X�^�[
	// �ݒ�
	piplineDesc.RasterizerState.CullMode = cullmord;	// �w��
	piplineDesc.RasterizerState.FillMode = fillmord;	// �|���S�����h��Ԃ�
	piplineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O

	//// �u�����f�B���O
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = piplineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	Blending(blenddesc);

	// ���_���C�A�E�g�̐ݒ�
	piplineDesc.InputLayout.pInputElementDescs = inputLayout;
	piplineDesc.InputLayout.NumElements = inputLayoutSize;

	// �}�`�̌`��ݒ�
	piplineDesc.PrimitiveTopologyType = topologyType;

	// ���̑��̐ݒ�
	piplineDesc.NumRenderTargets = 1;
	piplineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	piplineDesc.SampleDesc.Count = 1;

	// �f�v�X�X�e���V���X�e�[�g�ݒ�
	if (isDeep)
	{
		piplineDesc.DepthStencilState.DepthEnable = true;
		piplineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	}
	else
	{
		piplineDesc.DepthStencilState.DepthEnable = false;
		piplineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	}
	piplineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	piplineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	SetRootSignature(4);

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	piplineDesc.pRootSignature = piplineSet.rootSignature.Get();

	// �p�C�v���C���X�e�[�g�̐���
	result = KDirectXCommon::GetInstance()->GetDev()->CreateGraphicsPipelineState(&piplineDesc, IID_PPV_ARGS(&piplineSet.pipelineState));
	assert(SUCCEEDED(result));
}

void KGPlin::Update(D3D12_PRIMITIVE_TOPOLOGY primitive)
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	KDirectXCommon::GetInstance()->GetCmdlist()->SetPipelineState(piplineSet.pipelineState.Get());
	KDirectXCommon::GetInstance()->GetCmdlist()->IASetPrimitiveTopology(primitive);
}

void KGPlin::Setting()
{
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootSignature(piplineSet.rootSignature.Get());
}
