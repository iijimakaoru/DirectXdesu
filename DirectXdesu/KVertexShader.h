#pragma once
#include "KDirectInit.h"
#include <d3dcompiler.h>

class KVertexShader
{
public:
	KVertexShader(KDirectInit dx);
	void Init(KDirectInit dx);
	void Error(KDirectInit dx);
	ID3D10Blob* vsBlob = nullptr; // ���_�V�F�[�_�[�I�u�W�F�N�g
	ID3D10Blob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g
};

