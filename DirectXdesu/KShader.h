#pragma once
#include "KDirectInit.h"
#include <d3dcompiler.h>

class Shader
{
public:
	Shader();
	Shader(KDirectInit dx);
	void VertexInit(KDirectInit dx);
	void VertexError(KDirectInit dx);
	void PixelInit(KDirectInit dx);
	void PixelError(KDirectInit dx);
	ID3D10Blob* vsBlob = nullptr; // ���_�V�F�[�_�[�I�u�W�F�N�g
	ID3D10Blob* psBlob = nullptr; // �s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ID3D10Blob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g
};

