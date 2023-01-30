#include "Obj.hlsli"

//VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
//{
//	VSOutput output;
//	output.svpos = mul(mat, pos);
//	output.normal = normal;
//	output.uv = uv;
//	return output;
//}

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	//// 右、下、奥の方向を向いたライト
	//float3 lightdir = float3(1, -1, 1);
	//lightdir = normalize(lightdir);
	//// ライトの色
	//float3 lightcolor = float3(1, 1, 1);

	// 法線にワールド行列によるスケーリング・回転を適用
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);

	// ピクセルシェーダーに渡す値
	VSOutput output;
	//output.svpos = mul(mat, pos);
	output.svpos = mul(mul(viewproj, world), pos);

	// Lambert反射の計算
	output.color.rgb = dot(lightv, wnormal.xyz)*m_diffuse*lightcolor;
	output.color.a = m_alpha;
	output.uv = uv;

	// 環境反射光
	float3 ambient = m_ambient;

	// 拡散反射光
	//float3 diffuse = dot(-lightdir, normal) * m_diffuse;
	float3 diffuse = dot(lightv, wnormal.xyz) * m_diffuse;

	// 視点座標
	//const float3 eye = float3(0, 0, -20);
	// 頂点から視点への方向ベクトル
	//float3 eyedir = normalize(eye - pos.xyz);
	float3 eyedir = normalize(cameraPos - wpos.xyz);

	// 光尺度
	const float shiniess = 4.0f;

	// 反射光ベクトル
	//float3 reflect = normalize(lightdir + 2 * dot(-lightdir, normal) * normal);
	float3 reflect = normalize(-lightv + 2 * dot(lightv, wnormal.xyz) * wnormal.xyz);

	// 鏡面反射光
	float3 specular = pow(saturate(dot(reflect, eyedir)), shiniess) * m_specular;

	// 全て加算する
	output.color.rgb = (ambient + diffuse + specular) * lightcolor;
	output.color.a = m_alpha;

	return output;
}