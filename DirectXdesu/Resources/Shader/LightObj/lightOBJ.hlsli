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
    float4 svpos : SV_POSITION; // システム用頂点座標
	
    float4 worldpos : POSITION; // ワールド座標
    
    float3 normal : NORMAL; // 法線ベクトル
	
    float2 uv : TEXCOORD; // uv値
};