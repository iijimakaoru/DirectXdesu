#include "lightOBJ.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    // 法線にワールド行列によるスケーリング・回転を適用
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);
    
    VSOutput output;
    output.svpos = mul(mul(viewPro, world), pos);
    float3 eyeDir = normalize(cameraPos - wpos.xyz);
    
    // 環境反射光
    float3 ambient = m_ambient;
    
    // 拡散反射光
    float3 diffuse = dot(lightDir, wnormal.xyz) * m_diffuse;
    
    // 反射光ベクトル
    float3 reflect = normalize(-lightDir + 2 * dot(lightDir, wnormal.xyz) * wnormal.xyz);
    
    // 光沢度
    const float shininess = 4.0f;
    
    // 鏡面反射光
    float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;
    
    // 全て加算
    output.color.rgb = (ambient + diffuse + reflect) * lightColor;
    output.color.a = m_alpha;
    output.uv = uv;
    
    return output;
}