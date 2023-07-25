#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); // 0番に設定されたテクスチャ
SamplerState smp : register(s0); // 0番に設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    // UV指定したピクセルの色をサンプリング
    float4 texColor = tex.Sample(smp, input.uv);
    
    // 出力結果格納変数
    float4 result = float4(texColor.rgb, 1.0f);
    
    // RGBの数値を出力
    return result;
}