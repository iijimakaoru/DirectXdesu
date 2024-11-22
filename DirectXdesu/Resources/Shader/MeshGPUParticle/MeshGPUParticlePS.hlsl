#include "MeshGPUParticle.hlsli"

float4 main(GS_OUTPUT input) : SV_TARGET
{
    input.UV = input.UV * 2 - 1;

    float fade = saturate(distance(float2(0, 0), input.UV));
    float3 color = lerp(input.Color.rgb, float3(0, 0, 0), fade * fade);

    return float4(color, 1);
}