#include "ReversalPostEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番に設定されたテクスチャ
SamplerState smp : register(s0); // 0番に設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    // UV指定したピクセルの色をサンプリング
    // 色反転
    float4 texColor0 = tex0.Sample(smp, input.uv);
    
    float4 color = 1 - texColor0;
    
    // RGBの数値を出力
    return float4((color.rgb) * 1.0f, 1);
}