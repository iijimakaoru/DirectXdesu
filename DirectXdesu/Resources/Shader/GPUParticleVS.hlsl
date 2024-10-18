#include "GPUParticleHeader.hlsli"

StructuredBuffer<Particle> gParticles : register(t0);
ConstantBuffer<PreView> gPreview : register(b0);

VSOutput main( VSInput input,uint instanceId : SV_InstanceID )
{
    VSOutput output;
    Particle particle = gParticles[instanceId];
    
    float4x4 worldMatrix = gPreview.billbordMatrix;
    worldMatrix[0] *= particle.scale.x;
    worldMatrix[1] *= particle.scale.y;
    worldMatrix[2] *= particle.scale.z;
    worldMatrix[3].xyz = particle.translate;
    
    output.position = input.position;
    output.texcoord = input.texcoord;
    output.color = particle.color;
    
    return output;
}