#include "GPUParticleHeader.hlsli"
#include "SimplexNoise.hlsli"

RWStructuredBuffer<Particle> ParticlePool : register(u0);
ConsumeStructuredBuffer<uint> CDeadList : register(u1);
RWStructuredBuffer<ParticleDraw> DrawList : register(u2);
RWStructuredBuffer<uint> DrawArgs : register(u3);

[numthreads(32, 1, 1)]
void main(uint id : SV_DispatchThreadID)
{
    // 生成上限
    if (id.x >= (uint) emitCount)
        return;

    // 
    uint emitIndex = CDeadList.Consume();

    float3 gridPosition;
    uint gridIndex = emitIndex;
    gridPosition.x = gridIndex % (gridSize + 1);
    gridIndex /= (gridSize + 1);
    gridPosition.y = gridIndex % (gridSize + 1);
    gridIndex /= (gridSize + 1);
    gridPosition.z = gridIndex;

	// ParticlePool で更新します
    Particle emitParticle = ParticlePool.Load(emitIndex);

	// 色と位置はグリッドの位置とサイズによって異なります
    emitParticle.Position = gridPosition / 10.0f - float3(gridSize / 20.0f, gridSize / 20.0f, -gridSize / 10.0f);
    emitParticle.Velocity = float3(0, 0.0f, 0.0f);
    emitParticle.Color = float4(gridPosition / gridSize, 1);
    emitParticle.Age = 0.0f;
    emitParticle.Size = 0.5f;
    emitParticle.Alive = 1.0f;

	//Put it back
    ParticlePool[emitIndex] = emitParticle;
}