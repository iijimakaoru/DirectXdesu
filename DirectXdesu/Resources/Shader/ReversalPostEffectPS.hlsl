#include "ReversalPostEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0�Ԃɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�Ԃɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    // UV�w�肵���s�N�Z���̐F���T���v�����O
    // �F���]
    float4 texColor0 = tex0.Sample(smp, input.uv);
    
    float4 color = 1 - texColor0;
    
    // RGB�̐��l���o��
    return float4((color.rgb) * 1.0f, 1);
}