// �}�e���A��
cbuffer cbuff0 : register(b0)
{
	float4 color;
	//matrix mat;
};

// ���_�V�F�[�_�[�̏o�͍\����
struct VSOutput
{
	// �V�X�e���p���_���W
	float4 svpos : SV_POSITION;
	// UV�l
	float2 uv : TEXCOORD;
};