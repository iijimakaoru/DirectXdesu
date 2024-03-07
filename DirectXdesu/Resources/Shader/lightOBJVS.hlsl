#include "lightOBJ.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    // 法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);
    
    VSOutput output;
    output.svpos = mul(mul(viewPro, world), pos);
    
    // 環境反射光
    float3 ambient = color.rgb * m_ambient;
    // 拡散反射光
    float3 diffuse = dot(lightv, wnormal.xyz) * m_diffuse;
    // 光沢度
    const float shininess = 100.0f;
    // 頂点から視点へのベクトル
    float3 eyeDir = normalize(cameraPos - wpos.xyz);
    // 反射光ベクトル
    float3 reflect = normalize(-lightv + 2 * dot(lightv, wnormal.xyz) * wnormal.xyz);
    // 鏡面反射光
    float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;
    
    // 全て加算
    output.color.rgb = (ambient + diffuse + reflect) * lightColor;
    output.color.a = color.a * m_alpha;
    output.uv = uv;
    
    return output;
}