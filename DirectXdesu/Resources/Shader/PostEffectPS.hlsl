#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); // 0�Ԃɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�Ԃɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, input.uv);
    
    return float4(texColor.rgb, 1);
}