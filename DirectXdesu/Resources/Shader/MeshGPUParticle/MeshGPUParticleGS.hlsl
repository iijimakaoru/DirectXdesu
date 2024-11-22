#include "MeshGPUParticle.hlsli"

[maxvertexcount(4)]
void main(point VS_OUTPUT input[1], inout TriangleStream<GS_OUTPUT> outStream)
{
    GS_OUTPUT output;

    float2 offsets[4];
    offsets[0] = float2(-1.0f, -1.0f);
    offsets[1] = float2(-1.0f, +1.0f);
    offsets[2] = float2(+1.0f, -1.0f);
    offsets[3] = float2(+1.0f, +1.0f);

    matrix mvp = mul(mul(world, view), projection);

	[unroll]
    for (int i = 0; i < 4; i++)
    {
        output.Position = mul(float4(input[0].Position, 1.0f), mvp);

        float depthChange = output.Position.z / output.Position.w;

		// Adjust based on depth (prevents particles from getting too large)
        offsets[i].y *= aspectRatio;
        output.Position.xy += offsets[i] * depthChange * input[0].Size;
        output.Color = input[0].Color;
        output.UV = saturate(offsets[i]);

		// Done
        outStream.Append(output);
    }
}