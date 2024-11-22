// パーティクル情報
struct Particle
{
    float4 Color;
    float3 Position;
    float Age;
    float3 Velocity;
    float Size;
    float Alive;
    float3 Padding;
};

// 描画
struct ParticleDraw
{
    uint Index;
}; 

// 頂点出力
struct VS_OUTPUT
{
    float3 Position : POSITION;
    float Size : SIZE;
    float4 Color : COLOR;
};

// ジオメトリ出力
struct GS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

float CalculateGridPosition(uint index, int gridSize)
{
    float3 gridPosition;

    gridSize += 1;

    gridPosition.x = index % gridSize;

    index /= gridSize;
    gridPosition.y = index % gridSize;

    index /= gridSize;
    gridPosition.z = index;

    return gridPosition;
}

cbuffer objectData : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
    float aspectRatio;
};

cbuffer timeData : register(b1)
{
    float deltaTime;
    float TotalTime;
}

cbuffer particleData : register(b2)
{
    float4 startColor;
    float4 endColor;
    float3 velocity;
    float lifeTime;
    float3 acceleration;
    float pad;
    int emitCount;
    int maxParticles;
    int gridSize;
}