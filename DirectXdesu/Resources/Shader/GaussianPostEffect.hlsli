// 頂点シェーダーの出力構造体
struct VSOutput
{
	// システム用頂点座標
    float4 svpos : SV_POSITION;
	// UV値
    float2 uv : TEXCOORD;
};