#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番に設定されたテクスチャ
Texture2D<float4> tex1 : register(t1); // 1番に設定されたテクスチャ
SamplerState smp : register(s0); // 0番に設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    // UV指定したピクセルの色をサンプリング
    // 色反転
    float4 texColor0 = float4(1, 1, 1, 1) - tex0.Sample(smp, input.uv);
    // ぼかし
    float4 texColor1 = tex1.Sample(smp, input.uv);
    
    
    
    float4 color = texColor0;
    if (fmod(input.uv.y,0.1f) < 0.05f)
    {
        color = texColor1;
    }
    
    // RGBの数値を出力
    return float4((color.rgb) * 1.0f, 1);
}