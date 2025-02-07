#include "MeshGPUParticle.hlsli"
#include "MeshSimplexNoise.hlsli"

RWStructuredBuffer<Particle> ParticlePool : register(u0);
AppendStructuredBuffer<uint> ADeadList : register(u1);
RWStructuredBuffer<ParticleDraw> DrawList : register(u2);
RWStructuredBuffer<uint> DrawArgs : register(u3);

[numthreads(1024, 1, 1)]
void main(uint id : SV_DispatchThreadID)
{
    if (id.x >= (uint) maxParticles)
    {
        return;
    }

    Particle particle = ParticlePool.Load(id.x);

    if (particle.Alive == 0.0f)
    {
        return;
    }
	
    particle.Alive = (float) (particle.Age < lifeTime);
	
    particle.Position += particle.Velocity * deltaTime;
	
    particle.Velocity = velocity;
    
    particle.Size = size;
    
    particle.Color = startColor;
    
    particle.Age += deltaTime;

	// 粒子を元に戻す
    ParticlePool[id.x] = particle;

	// 新しく死んだ？
    if (particle.Alive <= 0.0f)
    {
		// デッドリストに追加
        ADeadList.Append(id.x);
    }
    else
    {
		// 描画リストのカウンターをインクリメントし、返された (インクリメント前の) インデックスに新しい描画データを置きます。
        uint drawIndex = DrawList.IncrementCounter();

		// 描画データを設定する
        ParticleDraw drawData;
        drawData.Index = id.x; // このパーティクルの実際のインデックス

        DrawList[drawIndex] = drawData;
    }
}