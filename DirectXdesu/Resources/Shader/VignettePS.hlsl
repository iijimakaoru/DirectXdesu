#include"Vignette.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
     //�e�N�X�`���̐F�𓾂�
    float4 textureColor = tex.Sample(smp, input.uv);
    
    //�~�͈̔͂��v�Z
    float vignette = length(float2(0.5,0.5) - input.uv);
    vignette = clamp(vignette - 0.4, 0, 1);
    
    textureColor.rgb += (vignette) * float3(1, 0, 0);

    return float4(textureColor.rgb, 1.0);
}