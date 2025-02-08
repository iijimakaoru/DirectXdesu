#include "MeshGPUParticle.hlsli"
#include "MeshSimplexNoise.hlsli"

RWStructuredBuffer<Particle> ParticlePool : register(u0);
ConsumeStructuredBuffer<uint> CDeadList : register(u1);
RWStructuredBuffer<ParticleDraw> DrawList : register(u2);
RWStructuredBuffer<uint> DrawArgs : register(u3);
RWStructuredBuffer<Mesh> meshes : register(u4);

StructuredBuffer<Mesh> meshs : register(t0);

[numthreads(1024, 1, 1)]
void main(uint id : SV_DispatchThreadID)
{
    if (id.x >= (uint) meshSize)
    {
        return;
    }

    uint emitIndex = CDeadList.Consume();

	// ParticlePool‚ğƒ[ƒh
    Particle emitParticle = (Particle)0;

	// î•ñ‚ğ“ü—Í
    emitParticle.Position = meshs[id.x].pos;
    emitParticle.Velocity = velocity;
    emitParticle.Color = startColor;
    emitParticle.Age = 0.0f;
    emitParticle.Size = size;
    emitParticle.Alive = 1.0f;

	// Œ³‚É–ß‚µ‚Ä‚­‚¾‚³‚¢
    ParticlePool[emitIndex] = emitParticle;
}