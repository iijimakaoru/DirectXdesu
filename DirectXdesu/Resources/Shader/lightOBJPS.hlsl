#include "lightOBJ.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    // �e�N�X�`���}�b�s���O
    float4 texColor = float4(tex.Sample(smp, input.uv)) * color;
    // �V�F�[�f�B���O�ɂ��F
    float4 shaderColor;
    // ����x
    const float shininess = 4.0f;
     // ���_���王�_�ւ̃x�N�g��
    float3 eyeDir = normalize(cameraPos - input.worldpos.xyz);
    // ���C�g�Ɍ������x�N�g���Ɩ@���̓���
    float3 dotLightNormal = dot(lightv, input.normal.xyz);
    // ���ˌ��x�N�g��
    float3 reflect = normalize(-lightv + 2 * dotLightNormal * input.normal);
    // �����ˌ�
    float3 ambient = m_ambient;
    // �g�U���ˌ�
    float3 diffuse = dotLightNormal * m_diffuse;
    // ���ʔ��ˌ�
    float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;
    // �S�ĉ��Z
    shaderColor.rgb = ((ambient + diffuse + specular)) * lightColor;
    shaderColor.a = m_alpha;
    // �V�F�[�f�B���O�ɂ��F�ŕ`��
    return (shaderColor * texColor);
}