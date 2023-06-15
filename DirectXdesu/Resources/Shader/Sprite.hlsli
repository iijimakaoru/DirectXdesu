// マテリアル
cbuffer cbuff0 : register(b0)
{
	float4 color;
	matrix mat;
};

// 頂点シェーダーの出力構造体
// (頂点シェーダーからピクセルシェーダーへのやり取り用)
struct VSOutput
{
	// システム用頂点座標
	float4 svpos : SV_POSITION;
	// uv値
	float2 uv : TEXCOORD;
};