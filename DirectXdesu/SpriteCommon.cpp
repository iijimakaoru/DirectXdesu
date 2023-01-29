#include "SpriteCommon.h"

SpriteCommon::SpriteCommon()
{
	Init();
}

SpriteCommon* SpriteCommon::GetInstance()
{
	static SpriteCommon instance;
	return &instance;
}

void SpriteCommon::Init()
{
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	D3D12_APPEND_ALIGNED_ELEMENT,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},		//	xyzç¿ïW
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},				//	uvç¿ïW
	};
	shader.Init(L"SpriteVS.hlsl", L"SpritePS.hlsl");
	pipeline.Init(shader, inputLayout, _countof(inputLayout), D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK, false);
	pipeline.SetBlending(KGPlin::ALPHA);

	mat2D.Identity();
	mat2D.m[0][0] = 2.0f / KWinApp::GetWindowSizeW();
	mat2D.m[1][1] = -2.0f / KWinApp::GetWindowSizeH();
	mat2D.m[3][0] = -1;
	mat2D.m[3][1] = 1;
}

void SpriteCommon::Draw()
{
	pipeline.Setting();
	pipeline.Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


