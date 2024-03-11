#include "lightOBJ.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    // テクスチャマッピング
    float4 texColor = float4(tex.Sample(smp, input.uv)) * color;
    // シェーディングによる色
    float4 shaderColor;
    // 光沢度
    const float shininess = 4.0f;
     // 頂点から視点へのベクトル
    float3 eyeDir = normalize(cameraPos - input.worldpos.xyz);
    // ライトに向かうベクトルと法線の内積
    float3 dotLightNormal = dot(lightv, input.normal.xyz);
    // 反射光ベクトル
    float3 reflect = normalize(-lightv + 2 * dotLightNormal * input.normal);
    // 環境反射光
    float3 ambient = m_ambient;
    // 拡散反射光
    float3 diffuse = dotLightNormal * m_diffuse;
    // 鏡面反射光
    float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;
    // 全て加算
    shaderColor.rgb = ((ambient + diffuse + specular)) * lightColor;
    shaderColor.a = m_alpha;
    // シェーディングによる色で描画
    return (shaderColor * texColor);
}