#include "MeshGPUParticle.hlsli"
#include "MeshSimplexNoise.hlsli"

RWStructuredBuffer<Particle> ParticlePool : register(u0);
ConsumeStructuredBuffer<uint> CDeadList : register(u1);
RWStructuredBuffer<ParticleDraw> DrawList : register(u2);
RWStructuredBuffer<uint> DrawArgs : register(u3);
RWStructuredBuffer<Mesh> meshes : register(u4);

StructuredBuffer<Mesh> meshs : register(t0);

[numthreads(32, 1, 1)]
void main(uint id : SV_DispatchThreadID)
{
    if (id.x >= (uint) emitCount)
        return;
    
    if (id.x >= (uint) meshSize)
        return;

    uint emitIndex = CDeadList.Consume();

	// update it in ParticlePool
    Particle emitParticle = ParticlePool.Load(emitIndex);

	//color and position depend on the grid position and size
    emitParticle.Position = meshs[id.x].pos;
    emitParticle.Velocity = float3(0, 0.0f, 0.0f);
    emitParticle.Color = color;
    emitParticle.Age = 0.0f;
    emitParticle.Size = 0.1f;
    emitParticle.Alive = 1.0f;

	//Put it back
    ParticlePool[emitIndex] = emitParticle;
}