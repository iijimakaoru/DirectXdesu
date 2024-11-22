#include "MeshGPUParticle.hlsli"

struct VS_OUTPUT
{
    float3 Position : POSITION;
    float Size : SIZE;
    float4 Color : COLOR;
};

StructuredBuffer<Particle> ParticlePool : register(t0);
StructuredBuffer<ParticleDraw> DrawList : register(t1);

VS_OUTPUT main(uint id : SV_VertexID)
{
    VS_OUTPUT output;

    ParticleDraw draw = DrawList.Load(id);
    Particle particle = ParticlePool.Load(draw.Index);

	// pass through
    output.Position = particle.Position;
    output.Size = particle.Size;
    output.Color = particle.Color;

    return output;
}