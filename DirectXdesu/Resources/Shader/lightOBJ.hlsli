cbuffer cbuff0 : register(b0)
{
    float4 color;
    matrix viewPro;
    matrix world;
    float3 cameraPos;
}

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
    float3 lightColor;
}

struct VSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	
    float4 worldpos : POSITION; // ���[���h���W
    
    float3 normal : NORMAL; // �@���x�N�g��
	
    float2 uv : TEXCOORD; // uv�l
};