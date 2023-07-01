#include "KGPlin.h"
#include "KDirectXCommon.h"

void KGPlin::SetShader(KShader shader)
{
	// Vertex
	piplineDesc.VS.pShaderBytecode = shader.GetVSBlob()->GetBufferPointer();
	piplineDesc.VS.BytecodeLength = shader.GetVSBlob()->GetBufferSize();

	// Pixcel
	piplineDesc.PS.pShaderBytecode = shader.GetPSBlob()->GetBufferPointer();
	piplineDesc.PS.BytecodeLength = shader.GetPSBlob()->GetBufferSize();
}

void KGPlin::SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY shaderVisibility)
{
	rootParam.ParameterType = type;
	rootParam.Descriptor.ShaderRegister = shaderRegister;
	rootParam.Descriptor.RegisterSpace = registerSpace;
	rootParam.ShaderVisibility = shaderVisibility;
}

void KGPlin::SetRootParam(D3D12_ROOT_PARAMETER& rootParam, D3D12_ROOT_PARAMETER_TYPE type, D3D12_DESCRIPTOR_RANGE pDescripterRange, UINT numDescripterRanges, D3D12_SHADER_VISIBILITY shaderVisibility)
{
	rootParam.ParameterType = type;
	rootParam.DescriptorTable.pDescriptorRanges = &pDescripterRange;
	rootParam.DescriptorTable.NumDescriptorRanges = numDescripterRanges;
	rootParam.ShaderVisibility = shaderVisibility;
}

void KGPlin::SetRootSignature(UINT rootParamNum)
{
	HRESULT result;

	// �f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descripterRange{};
	descripterRange.NumDescriptors = 1;
	descripterRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descripterRange.BaseShaderRegister = 0;
	descripterRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//// ���[�g�p�����[�^
	// �ݒ�
	std::vector<D3D12_ROOT_PARAMETER> rootParams = {};
	rootParams.resize(rootParamNum + 1);
	SetRootParam(rootParams[0], D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, descripterRange, 1);
	for (size_t i = 0; i < rootParamNum; i++)
	{
		SetRootParam(rootParams[i + 1], D3D12_ROOT_PARAMETER_TYPE_CBV, static_cast<UINT>(i), 0);
	}

	//// �T���v���[
	// �e�N�X�`���T���v���[�̐ݒ�
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

	//// ���[�g�V�O�l�`��
	// �ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = static_cast<UINT>(rootParams.size());
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// �V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	ComPtr<ID3DBlob> errorBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, rootSigBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
	assert(SUCCEEDED(result));
	result = KDirectXCommon::GetInstance()->GetDev()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));
}

void KGPlin::SetScreenRootSignature()
{
}

void KGPlin::Blending(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, const int mord)
{
	//	���ʐݒ�
	if (mord != NONE) {
		piplineDesc.BlendState.AlphaToCoverageEnable = true;
		blendDesc.BlendEnable = true;
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	}
	else {
		piplineDesc.BlendState.AlphaToCoverageEnable = false;
	}

	switch (mord)
	{
	case ADD:
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_ONE;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case SUB:
		blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc.SrcBlend = D3D12_BLEND_ONE;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case INV:
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc.DestBlend = D3D12_BLEND_ZERO;
		break;
	case ALPHA:
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;
	default:
		break;
	}
}

void KGPlin::CreatePipelineAll(KShader shader, std::string shaderName)
{
	HRESULT result;

	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDev();

	// �V�F�[�_�[�ݒ�
	SetShader(shader);

	// Obj�V�F�[�_�[
	if (shaderName == "Obj")
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

		// �T���v���}�X�N�̐ݒ�
		piplineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// ���X�^���C�U�̐ݒ�
		piplineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // �w�ʂ��J�����O
		piplineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S���h��Ԃ�
		piplineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O

		// �u�����h�X�e�[�g
		piplineDesc.BlendState.RenderTarget[0];

		// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
		D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = piplineDesc.BlendState.RenderTarget[0];
		blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		blenddesc.BlendEnable = true;
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

		// ����������
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

		// ���_���C�A�E�g�̐ݒ�
		piplineDesc.InputLayout.pInputElementDescs = inputLayout;
		piplineDesc.InputLayout.NumElements = _countof(inputLayout);

		// �}�`�̌`��ݒ�
		piplineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// �[�x�X�e���V���X�e�[�g
		piplineDesc.DepthStencilState.DepthEnable = true; // �[�x�e�X�g
		piplineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // �������݋���
		piplineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // ��������΍��i
		piplineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g
		
		// ���̑��̐ݒ�
		piplineDesc.NumRenderTargets = 1;
		piplineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		piplineDesc.SampleDesc.Count = 1;

		// �f�X�N���v�^�����W�̐ݒ�
		D3D12_DESCRIPTOR_RANGE descriptorRange{};
		descriptorRange.NumDescriptors = 1; // ��x�̕`��Ŏg���e�N�X�`����
		descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptorRange.BaseShaderRegister = 0; // �e�N�X�`�����W�X�^0��
		descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// ���[�g�p�����[�^�̐ݒ�
		D3D12_ROOT_PARAMETER rootParams[4] = {};

		// �e�N�X�`�����W�X�^0��
		rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // ���
		rootParams[0].DescriptorTable.pDescriptorRanges = &descriptorRange;
		rootParams[0].DescriptorTable.NumDescriptorRanges = 1;					  // �f�X�N���v�^�����W��
		rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			  // �S�ẴV�F�[�_�[���猩����

		// �萔�o�b�t�@0��
		rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // ���
		rootParams[1].Descriptor.ShaderRegister = 0;				  // �萔�o�b�t�@�ԍ�
		rootParams[1].Descriptor.RegisterSpace = 0;					  // �f�t�H���g�l
		rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // �S�ẴV�F�[�_�[���猩����

		// �萔�o�b�t�@1��
		rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // ���
		rootParams[2].Descriptor.ShaderRegister = 1;				  // �萔�o�b�t�@�ԍ�
		rootParams[2].Descriptor.RegisterSpace = 0;					  // �f�t�H���g�l
		rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // �S�ẴV�F�[�_�[���猩����

		// �萔�o�b�t�@2��
		rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // ���
		rootParams[3].Descriptor.ShaderRegister = 2;				  // �萔�o�b�t�@�ԍ�
		rootParams[3].Descriptor.RegisterSpace = 0;					  // �f�t�H���g�l
		rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // �S�ẴV�F�[�_�[���猩����

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

		// ���[�g�V�O�l�`���̐ݒ�
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSignatureDesc.pParameters = rootParams; // ���[�g�p�����[�^�̐擪�A�h���X
		rootSignatureDesc.NumParameters = _countof(rootParams); // ���[�g�p�����[�^��
		rootSignatureDesc.pStaticSamplers = &samplerDesc;
		rootSignatureDesc.NumStaticSamplers = 1;

		// ���[�g�V�O�l�`���̃V���A���C�Y
		ComPtr<ID3DBlob> rootSigBlob;
		ComPtr<ID3DBlob> errorBlob;
		result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
			rootSigBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
		assert(SUCCEEDED(result));
		result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
			IID_PPV_ARGS(&rootSignature));
		assert(SUCCEEDED(result));

		// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
		piplineDesc.pRootSignature = rootSignature.Get();

		result = device->CreateGraphicsPipelineState(&piplineDesc, IID_PPV_ARGS(&pipelineState));
		assert(SUCCEEDED(result));
	}
	// Sprite�V�F�[�_�[
	else if (shaderName == "Sprite")
	{
#pragma region ���_���C�A�E�g�z��̐錾�Ɛݒ�
		static D3D12_INPUT_ELEMENT_DESC inputLayout[] = 
		{
			{// xy���W
				"POSITION",										// �Z�}���e�B�b�N��
				0,												// �����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X
				DXGI_FORMAT_R32G32B32_FLOAT,					// �v�f���ƃr�b�g����\��
				0,												// ���̓X���b�g�C���f�b�N�X
				D3D12_APPEND_ALIGNED_ELEMENT,					// �f�[�^�̃I�t�Z�b�g
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		// ���̓f�[�^���
				0												// ��x�ɕ`�悷��C���X�^���X��
			},
			{ // uv���W(1�s�ŏ������ق������₷��)
				"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
		};
#pragma endregion
#pragma region �p�C�v���C���X�e�[�g�ݒ�ϐ��̐錾�Ɗe�퍀�ڂ̐ݒ�
		// �T���v���}�X�N�̐ݒ�
		piplineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// ���X�^���C�U�̐ݒ�
		piplineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �w�ʂ��J�����O���Ȃ�
		piplineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		piplineDesc.RasterizerState.DepthClipEnable = true;

		// �u�����h�X�e�[�g
		piplineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		// ���_���C�A�E�g�̐ݒ�
		piplineDesc.InputLayout.pInputElementDescs = inputLayout;
		piplineDesc.InputLayout.NumElements = _countof(inputLayout);

		// �}�`�̌`��ݒ�
		piplineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// ���̑��̐ݒ�
		piplineDesc.NumRenderTargets = 1;
		piplineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		piplineDesc.SampleDesc.Count = 1;

		// �[�x�X�e���V���X�e�[�g
		piplineDesc.DepthStencilState.DepthEnable = true; // �[�x�e�X�g
		piplineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // �������݋���
		piplineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // ��������΍��i
		piplineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g
		
		// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
		D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = piplineDesc.BlendState.RenderTarget[0];
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
		descriptorRange.NumDescriptors = 1; // ��x�̕`��Ŏg���e�N�X�`����
		descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptorRange.BaseShaderRegister = 0; // �e�N�X�`�����W�X�^0��
		descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// ���[�g�p�����[�^�̐ݒ�
		D3D12_ROOT_PARAMETER rootParams[3] = {};

		// �萔�o�b�t�@0��
		rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // ���
		rootParams[0].Descriptor.ShaderRegister = 0;				  // �萔�o�b�t�@�ԍ�
		rootParams[0].Descriptor.RegisterSpace = 0;					  // �f�t�H���g�l
		rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // �S�ẴV�F�[�_�[���猩����

		// �e�N�X�`�����W�X�^0��
		rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // ���
		rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
		rootParams[1].DescriptorTable.NumDescriptorRanges = 1;					  // �f�X�N���v�^�����W��
		rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			  // �S�ẴV�F�[�_�[���猩����

		// �萔�o�b�t�@1��
		rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // ���
		rootParams[2].Descriptor.ShaderRegister = 1;				  // �萔�o�b�t�@�ԍ�
		rootParams[2].Descriptor.RegisterSpace = 0;					  // �f�t�H���g�l
		rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // �S�ẴV�F�[�_�[���猩����

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
#pragma region ���[�g�V�O�l�`���̐���
		// ���[�g�V�O�l�`���̐ݒ�
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSignatureDesc.pParameters = rootParams; // ���[�g�p�����[�^�̐擪�A�h���X
		rootSignatureDesc.NumParameters = _countof(rootParams); // ���[�g�p�����[�^��
		rootSignatureDesc.pStaticSamplers = &samplerDesc;
		rootSignatureDesc.NumStaticSamplers = 1;

		// ���[�g�V�O�l�`���̃V���A���C�Y
		ComPtr<ID3DBlob> rootSigBlob;
		ComPtr<ID3DBlob> errorBlob;
		result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
			rootSigBlob.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());
		assert(SUCCEEDED(result));
		result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
			IID_PPV_ARGS(&rootSignature));
		assert(SUCCEEDED(result));

		// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
		piplineDesc.pRootSignature = rootSignature.Get();
#pragma endregion
#pragma region �O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐���
		result = device->CreateGraphicsPipelineState(&piplineDesc, IID_PPV_ARGS(&pipelineState));
		assert(SUCCEEDED(result));
#pragma endregion
	}
	// Fbx�V�F�[�_�[
	else if (shaderName == "Fbx")
	{
		// ���_���C�A�E�g
		D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ // xy���W(1�s�ŏ������ق������₷��)
				"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
				"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{ // uv���W(1�s�ŏ������ق������₷��)
				"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{// �e�����󂯂�{�[���ԍ�(4��)
				"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
			{// �{�[���X�L���E�F�C�g(4��)
				"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
		};

		// �T���v���}�X�N
		piplineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

		// ���X�^���C�U�X�e�[�g
		piplineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

		// �f�v�X�X�e���V���X�e�[�g
		piplineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

		// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
		D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
		blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA�S�Ẵ`�����l����`��
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

		// �u�����h�X�e�[�g�̐ݒ�
		piplineDesc.BlendState.RenderTarget[0] = blenddesc;

		// �[�x�o�b�t�@�̃t�H�[�}�b�g
		piplineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// ���_���C�A�E�g�̐ݒ�
		piplineDesc.InputLayout.pInputElementDescs = inputLayout;
		piplineDesc.InputLayout.NumElements = _countof(inputLayout);

		// �}�`�̌`��ݒ�i�O�p�`�j
		piplineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		piplineDesc.NumRenderTargets = 1;    // �`��Ώۂ�1��
		piplineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA(SRGB��)
		piplineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

		// �f�X�N���v�^�����W
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
		descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

		// ���[�g�p�����[�^
		CD3DX12_ROOT_PARAMETER rootparams[3] = {};

		// CBV(���W�ϊ��s��p)
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

		// SRV(�e�N�X�`��)
		rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

		// CBV(�X�L�j���O�p)
		rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

		// �X�^�e�B�b�N�T���v���[
		CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

		// ���[�g�V�O�l�`���̐ݒ�
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> rootSigBlob;
		ComPtr<ID3DBlob> errorBlob;

		// �o�[�W������������̃V���A���C�Y
		result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

		// ���[�g�V�O�l�`���̐���
		result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { assert(0); }

		piplineDesc.pRootSignature = rootSignature.Get();

		// �O���t�B�b�N�X�p�C�v���C���̐���
		result = device->CreateGraphicsPipelineState(&piplineDesc, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { assert(0); }
	}
	// PostEffect�V�F�[�_�[
	else if (shaderName == "PostEffect")
	{
		D3D12_INPUT_ELEMENT_DESC inputLayout[] =
		{
			{// xy���W
				"POSITION",										// �Z�}���e�B�b�N��
				0,												// �����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X
				DXGI_FORMAT_R32G32B32_FLOAT,					// �v�f���ƃr�b�g����\��
				0,												// ���̓X���b�g�C���f�b�N�X
				D3D12_APPEND_ALIGNED_ELEMENT,					// �f�[�^�̃I�t�Z�b�g
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		// ���̓f�[�^���
				0												// ��x�ɕ`�悷��C���X�^���X��
			},
			{ // uv���W(1�s�ŏ������ق������₷��)
				"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
		};

		// �T���v���}�X�N
		piplineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

		// ���X�^���C�U�X�e�[�g
		piplineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		piplineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		// �f�v�X�X�e���V���X�e�[�g
		piplineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		piplineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; // ��ɏ㏑��

		// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
		D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
		blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA�S�Ẵ`�����l����`��
		blenddesc.BlendEnable = true;
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

		// �u�����h�X�e�[�g�̐ݒ�
		piplineDesc.BlendState.RenderTarget[0] = blenddesc;

		// �[�x�o�b�t�@�̃t�H�[�}�b�g
		piplineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// ���_���C�A�E�g�̐ݒ�
		piplineDesc.InputLayout.pInputElementDescs = inputLayout;
		piplineDesc.InputLayout.NumElements = _countof(inputLayout);

		// �}�`�̌`��ݒ�i�O�p�`�j
		piplineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		piplineDesc.NumRenderTargets = 1;    // �`��Ώۂ�1��
		piplineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA(SRGB��)
		piplineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

		// �f�X�N���v�^�����W
		CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
		descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

		// ���[�g�p�����[�^
		CD3DX12_ROOT_PARAMETER rootparams[3] = {};

		// CBV(���W�ϊ��s��p)
		rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

		// SRV(�e�N�X�`��)
		rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

		// CBV(���W�ϊ��s��p)
		rootparams[2].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);

		// �X�^�e�B�b�N�T���v���[
		CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;

		// ���[�g�V�O�l�`���̐ݒ�
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> rootSigBlob;
		ComPtr<ID3DBlob> errorBlob;

		// �o�[�W������������̃V���A���C�Y
		result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
		if (FAILED(result)) { assert(0); }

		// ���[�g�V�O�l�`���̐���
		result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootSignature.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { assert(0); }

		piplineDesc.pRootSignature = rootSignature.Get();

		// �O���t�B�b�N�X�p�C�v���C���̐���
		result = device->CreateGraphicsPipelineState(&piplineDesc, IID_PPV_ARGS(pipelineState.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { assert(0); }
	}
	// �Ȃ���V�F�[�_�[�����ĂȂ��Ƃ�
	else
	{
		assert(0);
	}
}

void KGPlin::Update(D3D12_PRIMITIVE_TOPOLOGY primitive)
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	KDirectXCommon::GetInstance()->GetCmdlist()->SetPipelineState(pipelineState.Get());
	KDirectXCommon::GetInstance()->GetCmdlist()->IASetPrimitiveTopology(primitive);
}

void KGPlin::Setting()
{
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootSignature(rootSignature.Get());
}

void KGPlin::SetBlending(int mord)
{
	HRESULT result;
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = piplineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	Blending(blenddesc, mord);
	result = KDirectXCommon::GetInstance()->GetDev()->CreateGraphicsPipelineState(&piplineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
}
