#include "Basic.hlsli"

Texture2D<float4>tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET{
	float3 light = normalize(float3(1,-1,1)); // 右下奥向きのライト
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f; // 光源へのベクトルと法線ベクトルの内積
	float4 texcolor = float4(tex.Sample(smp, input.uv));
	return float4(texcolor.rgb * brightness, texcolor.a) * color; // 輝度をRGBに代入して出力
}