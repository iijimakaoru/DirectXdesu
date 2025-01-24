#include "GPUParticleHeader.hlsli"
#include "SimplexNoise.hlsli"

RWStructuredBuffer<Particle> ParticlePool : register(u0);
AppendStructuredBuffer<uint> ADeadList : register(u1);
RWStructuredBuffer<ParticleDraw> DrawList : register(u2);
RWStructuredBuffer<uint> DrawArgs : register(u3);

[numthreads(32, 1, 1)]
void main(uint id : SV_DispatchThreadID)
{
    if (id.x >= (uint) maxParticles)
        return;

    Particle particle = ParticlePool.Load(id.x);

    if (particle.Alive == 0.0f)
        return;

    particle.Age += deltaTime;
	
    particle.Alive = (float) (particle.Age < lifeTime);
	
    particle.Position += particle.Velocity * deltaTime;
	
    float3 curlPosition = particle.Position * 0.1f;
    float3 curlVelocity = curlNoise3D(curlPosition, 1.0f);
    particle.Velocity = curlVelocity * 2;

	//// 粒子を元に戻す
    ParticlePool[id.x] = particle;

	// 新しく死んだ？
    if (particle.Alive == 0.0f)
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