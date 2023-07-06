#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0�Ԃɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1); // 1�Ԃɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�Ԃɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    // UV�w�肵���s�N�Z���̐F���T���v�����O
    // �F���]
    float4 texColor0 = float4(1, 1, 1, 1) - tex0.Sample(smp, input.uv);
    // �ڂ���
    float4 texColor1 = tex1.Sample(smp, input.uv);
    
    
    
    float4 color = texColor0;
    if (fmod(input.uv.y,0.1f) < 0.05f)
    {
        color = texColor1;
    }
    
    // RGB�̐��l���o��
    return float4((color.rgb) * 1.0f, 1);
}