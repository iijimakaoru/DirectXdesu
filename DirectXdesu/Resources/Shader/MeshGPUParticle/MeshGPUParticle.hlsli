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

struct Mesh
{
    float3 pos;
    float3 normal;
};

struct ParticleDraw
{
    uint Index;
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
    float4 color;
    float3 velocity;
    float lifeTime;
    float3 acceleration;
    float pad;
    int emitCount;
    int maxParticles;
    int gridSize;
    uint meshSize;
}

struct VS_OUTPUT
{
    float3 Position : POSITION;
    float Size : SIZE;
    float4 Color : COLOR;
};

struct GS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};