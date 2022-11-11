cbuffer cbuff0 : register(b0)
{
	matrix mat;
};

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient : packoffset(c0);
	float3 m_diffuse : packoffset(c1);
	float3 m_specular : packoffset(c2);
	float m_alpha : packoffset(c2.w);
};

struct VSOutput {
	// �V�X�e���p���_���W
	float4 svpos : SV_POSITION;
	// �@���x�N�g��
	float3 normal : NORMAL;
	// uv�l
	float2 uv : TEXCOORD;
};