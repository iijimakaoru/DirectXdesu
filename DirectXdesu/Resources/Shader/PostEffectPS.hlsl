#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); // 0番に設定されたテクスチャ
SamplerState smp : register(s0); // 0番に設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    // UV指定したピクセルの色をサンプリング
    float4 texColor = tex.Sample(smp, input.uv);
    
    float4 maxColor = (1, 1, 1, 1);
    
    // RGBの数値を2倍にして出力
    return float4((texColor.rgb) * 1.0f, 1);
}