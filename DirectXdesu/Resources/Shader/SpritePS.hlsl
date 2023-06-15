#include "Sprite.hlsli"

Texture2D<float4> tex : register(t0); // 0�Ԃɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�Ԃɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    return float4(tex.Sample(smp, input.uv)) * color;
}