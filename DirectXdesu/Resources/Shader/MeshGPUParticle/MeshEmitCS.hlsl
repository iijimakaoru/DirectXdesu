#include "MeshGPUParticle.hlsli"
#include "MeshSimplexNoise.hlsli"

RWStructuredBuffer<Particle> ParticlePool : register(u0);
ConsumeStructuredBuffer<uint> CDeadList : register(u1);
RWStructuredBuffer<ParticleDraw> DrawList : register(u2);
RWStructuredBuffer<uint> DrawArgs : register(u3);
RWStructuredBuffer<Mesh> meshes : register(u4);

[numthreads(32, 1, 1)]
void main(uint id : SV_DispatchThreadID)
{
    if (id.x >= (uint) emitCount)
        return;
    
    if (id.x >= (uint) meshSize)
        return;

    uint emitIndex = CDeadList.Consume();

    float3 gridPosition;
    uint gridIndex = emitIndex;
    gridPosition.x = gridIndex % (gridSize + 1);
    gridIndex /= (gridSize + 1);
    gridPosition.y = gridIndex % (gridSize + 1);
    gridIndex /= (gridSize + 1);
    gridPosition.z = gridIndex;

	// update it in ParticlePool
    Particle emitParticle = ParticlePool.Load(emitIndex);

	//color and position depend on the grid position and size
    emitParticle.Position = meshes[id.x];
    emitParticle.Velocity = float3(0, 0.0f, 0.0f);
    emitParticle.Color = float4(gridPosition / gridSize, 1);
    emitParticle.Age = 0.0f;
    emitParticle.Size = 0.5f;
    emitParticle.Alive = 1.0f;

	//Put it back
    ParticlePool[emitIndex] = emitParticle;
}