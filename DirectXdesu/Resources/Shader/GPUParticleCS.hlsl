#include "GPUParticleHeader.hlsli"

static const uint kMaxParticles = 1024;
RWStructuredBuffer<Particle> gparticles : register(u0);
[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint particleIndex = DTid.x;
    if (particleIndex < kMaxParticles)
    {
        gparticles[particleIndex] = (Particle) 0;
    }
}