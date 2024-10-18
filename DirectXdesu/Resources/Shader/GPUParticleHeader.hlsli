// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 position;
    float2 texcoord;
    float4 color;
};

struct VSInput
{
    float3 position;
    float2 texcoord;
};

struct Particle
{
    float3 translate;
    float3 scale;
    float lifeTime;
    float3 velocity;
    float currentTime;
    float4 color;
};

struct PreView
{
    float4x4 viewProjection;
    float4x4 billbordMatrix;
};