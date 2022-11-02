#include "Sprite.hlsli"

//VSOutput main( float4 pos : POSITION, float2 uv : TEXCOORD)
//{
//	VSOutput output;
//	output.svpos = mul(mat, pos);
//	output.uv = uv;
//
//	return output;
//}

float4 main(float4 pos : POSITION) : SV_POSITION
{
	return pos;
}