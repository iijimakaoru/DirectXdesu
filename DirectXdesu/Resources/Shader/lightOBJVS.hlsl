#include "lightOBJ.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    // �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);
    
    VSOutput output;
    output.svpos = mul(mul(viewPro, world), pos);
    
    // �����ˌ�
    float3 ambient = color.rgb * m_ambient;
    // �g�U���ˌ�
    float3 diffuse = dot(lightv, wnormal.xyz) * m_diffuse;
    // ����x
    const float shininess = 100.0f;
    // ���_���王�_�ւ̃x�N�g��
    float3 eyeDir = normalize(cameraPos - wpos.xyz);
    // ���ˌ��x�N�g��
    float3 reflect = normalize(-lightv + 2 * dot(lightv, wnormal.xyz) * wnormal.xyz);
    // ���ʔ��ˌ�
    float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;
    
    // �S�ĉ��Z
    output.color.rgb = (ambient + diffuse + reflect) * lightColor;
    output.color.a = color.a * m_alpha;
    output.uv = uv;
    
    return output;
}