#pragma once
#include "KDirectInit.h"
#include <d3dcompiler.h>
#include "ShaderManager.h"

class KPixelShader 
{
public:
	KPixelShader();
	KPixelShader(KDirectInit dx);
	void Init(KDirectInit dx);
	void Error(KDirectInit dx);
	ID3D10Blob* psBlob = nullptr; // �s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ID3D10Blob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g
};

