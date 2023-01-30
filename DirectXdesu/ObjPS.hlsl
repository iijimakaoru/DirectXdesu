#include "Obj.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

//float4 main(VSOutput input) : SV_TARGET
//{
//	float3 light = normalize(float3(1,-1,1));
//	float light_diffuse = saturate(dot(-light, input.normal));
//	float3 shade_color;
//	shade_color = m_ambient;
//	shade_color += m_diffuse * light_diffuse;
//	float4 texcolor = tex.Sample(smp, input.uv);
//
//	return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha);
//}

//float4 main(VSOutput input) : SV_TARGET
//{
//	float4 texcolor = tex.Sample(smp,input.uv);
//
//	return input.color;
//}

float4 main(VSOutput input) : SV_TARGET
{
	// �e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp,input.uv);
	// �V�F�[�f�B���O�ɂ��F
	float4 shadecolor;
	// ���ړx
	const float shininess = 4.0f;
	// ���_���王�_�ւ̃x�N�g��
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
	float3 dotlightnormal = dot(lightv, input.normal);
	// ���ˌ��x�N�g��
	float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	// �����ˌ�
	float3 ambient = m_ambient;
	// �g�U���ˌ�
	float3 diffuse = dotlightnormal * m_diffuse;
	// ���ʔ��ˌ�
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	// �S�ĉ��Z
	shadecolor.rgb = (ambient + diffuse + specular) * lightcolor;
	shadecolor.a = m_alpha;

	return shadecolor*texcolor;
}