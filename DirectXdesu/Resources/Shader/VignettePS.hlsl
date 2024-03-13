#include"Vignette.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
     //テクスチャの色を得る
    float4 textureColor = tex.Sample(smp, input.uv);
    
    //円の範囲を計算
    float vignette = length((input.uv / size) - center / size);
    vignette = clamp(vignette, 0, 1);

    textureColor -= (vignette * power) * float4(color, 1.0f);

    return float4(textureColor.rgb, 1.0);
}