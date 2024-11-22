#include "MeshGPUParticle.hlsli"
#include "MeshSimplexNoise.hlsli"

RWStructuredBuffer<Particle> ParticlePool : register(u0);
AppendStructuredBuffer<uint> ADeadList : register(u1);
RWStructuredBuffer<ParticleDraw> DrawList : register(u2);
RWStructuredBuffer<uint> DrawArgs : register(u3);

[numthreads(32, 1, 1)]
void main(uint id : SV_DispatchThreadID)
{
	// outside range?
    if (id.x >= (uint) maxParticles) 
        return;

	// add the index to the dead list
    ADeadList.Append(id.x);
}