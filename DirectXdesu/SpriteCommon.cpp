#include "SpriteCommon.h"
#include "KDirectXCommon.h"

AhoSpriteCommon::AhoSpriteCommon()
{
	Init();
}

AhoSpriteCommon* AhoSpriteCommon::GetInstance()
{
	static AhoSpriteCommon instance;
	return &instance;
}

void AhoSpriteCommon::Init()
{
	HRESULT result;

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
	{// uv���W
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
	};
	shader.Init(L"Resources/Shader/SpriteVS.hlsl", L"Resources/Shader/SpritePS.hlsl");
	pipeline.Init(shader, inputLayout, _countof(inputLayout), 2, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE, false);
	pipeline.SetBlending(KGPlin::ALPHA);
}

void AhoSpriteCommon::Draw()
{
	pipeline.Setting();
	pipeline.Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


