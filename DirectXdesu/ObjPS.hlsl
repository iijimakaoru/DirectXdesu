#include "Obj.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

//float4 main(VSOutput input) : SV_TARGET
//{
//	float3 light = normalize(float3(1,-1,1));
//	float light_diffuse = saturate(dot(-light, input.normal));
//	float3 shade_color;
//	shade_color = m_ambient;
//	shade_color += m_diffuse * light_diffuse;
//	float4 texcolor = tex.Sample(smp, input.uv);
//
//	return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha);
//}

//float4 main(VSOutput input) : SV_TARGET
//{
//	float4 texcolor = tex.Sample(smp,input.uv);
//
//	return input.color;
//}

float4 main(VSOutput input) : SV_TARGET
{
	// テクスチャマッピング
	float4 texcolor = tex.Sample(smp,input.uv);
	// シェーディングによる色
	float4 shadecolor;
	// 光尺度
	const float shininess = 4.0f;
	// 頂点から視点へのベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	// ライトに向かうベクトルと法線の内積
	float3 dotlightnormal = dot(lightv, input.normal);
	// 反射光ベクトル
	float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	// 環境反射光
	float3 ambient = m_ambient;
	// 拡散反射光
	float3 diffuse = dotlightnormal * m_diffuse;
	// 鏡面反射光
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	// 全て加算
	shadecolor.rgb = (ambient + diffuse + specular) * lightcolor;
	shadecolor.a = m_alpha;

	return shadecolor*texcolor;
}