#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0�Ԃɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1); // 1�Ԃɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�Ԃɐݒ肳�ꂽ�T���v���[

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
    // UV�w�肵���s�N�Z���̐F���T���v�����O
    // �F���]
    float4 texColor0 =  tex0.Sample(smp, input.uv);
    
    // �ڂ���
    float totalWidth = 0, _Sigma = 0.005f, _StepWidth = 0.001f;
    float4 col = float4(0, 0, 0, 0);
    
    [loop]
    for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
    {
        [loop]
        for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
        {
            float2 pickUV = input.uv + float2(px, py);
            float weight = Gaussian(input.uv, pickUV, _Sigma);
            col += tex1.Sample(smp, pickUV) * weight;
            totalWidth += weight;
        }
    }
    
    col.rgb = col.rgb / totalWidth;
    col.a = 1;
    
    float4 texColor1 = col;
    
    float4 color = 1 - texColor0;
    if (fmod(input.uv.y,0.1f) < 0.05f)
    {
        color = texColor1;
    }
    
    // RGB�̐��l���o��
    return float4((color.rgb) * 1.0f, 1);
}