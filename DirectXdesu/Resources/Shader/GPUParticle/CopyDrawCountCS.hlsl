#include "GPUParticleHeader.hlsli"
#include "SimplexNoise.hlsli"

RWStructuredBuffer<Particle> ParticlePool : register(u0);
AppendStructuredBuffer<uint> ADeadList : register(u1);
RWStructuredBuffer<ParticleDraw> DrawList : register(u2);
RWStructuredBuffer<uint> DrawArgs : register(u3);

[numthreads(1, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
	// カウンタをインクリメントして前の値を取得します。
    // これは、描画したいパーティクルの数になります。
    DrawArgs[0] = DrawList.IncrementCounter(); // vertexCountPerInstance (or index count if using an index buffer)
    DrawArgs[1] = 1; // instanceCount
    DrawArgs[2] = 0; // offsets
    DrawArgs[3] = 0; // offsets
    DrawArgs[4] = 0; // offsets
    DrawArgs[5] = 0; // offsets
    DrawArgs[6] = 0; // offsets
    DrawArgs[7] = 0; // offsets
    DrawArgs[8] = 0; // offsets
}