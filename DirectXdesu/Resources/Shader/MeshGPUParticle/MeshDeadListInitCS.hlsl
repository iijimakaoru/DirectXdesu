#include "MeshGPUParticle.hlsli"
#include "MeshSimplexNoise.hlsli"

RWStructuredBuffer<Particle> ParticlePool : register(u0);
AppendStructuredBuffer<uint> ADeadList : register(u1);
RWStructuredBuffer<ParticleDraw> DrawList : register(u2);
RWStructuredBuffer<uint> DrawArgs : register(u3);

[numthreads(1024, 1, 1)]
void main(uint id : SV_DispatchThreadID)
{
	// 範囲外？
    if (id.x >= (uint) maxParticles) 
        return;

	// デッドリストにインデックスを追加する
    ADeadList.Append(id.x);
}