#include "lightOBJ.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    // �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);
    
    VSOutput output;
    output.svpos = mul(mul(viewPro, world), pos);
    float3 eyeDir = normalize(cameraPos - wpos.xyz);
    
    // �����ˌ�
    float3 ambient = m_ambient;
    
    // �g�U���ˌ�
    float3 diffuse = dot(lightDir, wnormal.xyz) * m_diffuse;
    
    // ���ˌ��x�N�g��
    float3 reflect = normalize(-lightDir + 2 * dot(lightDir, wnormal.xyz) * wnormal.xyz);
    
    // ����x
    const float shininess = 4.0f;
    
    // ���ʔ��ˌ�
    float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;
    
    // �S�ĉ��Z
    output.color.rgb = (ambient + diffuse + reflect) * lightColor;
    output.color.a = m_alpha;
    output.uv = uv;
    
    return output;
}