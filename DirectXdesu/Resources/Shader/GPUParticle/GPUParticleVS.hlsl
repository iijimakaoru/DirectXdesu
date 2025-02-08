#include "GPUParticleHeader.hlsli"

StructuredBuffer<Particle> ParticlePool : register(t0);
StructuredBuffer<ParticleDraw> DrawList : register(t1); 

VS_OUTPUT main(uint id : SV_VertexID)
{
    VS_OUTPUT output;

    ParticleDraw draw = DrawList.Load(id);
    Particle particle = ParticlePool.Load(draw.Index);

	// ’Ê‚è”²‚¯‚é
    output.Position = particle.Position;
    output.Size = particle.Size;
    output.Color = particle.Color;

    return output;
}