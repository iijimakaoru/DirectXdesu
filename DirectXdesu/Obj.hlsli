cbuffer cbuff0 : register(b0)
{
	//matrix mat;
	matrix viewproj; // �r���[�v���W�F�N�V����
	matrix world; // ���[���h�s��
	float3 cameraPos; // �J�������W(���[���h���W)
};

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient : packoffset(c0);
	float3 m_diffuse : packoffset(c1);
	float3 m_specular : packoffset(c2);
	float m_alpha : packoffset(c2.w);
};

cbuffer cbuff2 : register(b2)
{
	float3 lightv;
	float3 lightcolor;
}

struct VSOutput {
	// �V�X�e���p���_���W
	float4 svpos : SV_POSITION;
	// �@���x�N�g��
	//float3 normal : NORMAL;
	float4 color : COLOR;
	// uv�l
	float2 uv : TEXCOORD;
};