#pragma once
#include "KDirectInit.h"
#include <d3dcompiler.h>

class KShader
{
public:
	KShader();
	void VSNormal();
	void VSError();
	void PSNormal();
	void PSError();

	ID3D10Blob* vsBlob = nullptr; // ���_�V�F�[�_�[�I�u�W�F�N�g
	ID3D10Blob* psBlob = nullptr;
	ID3D10Blob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	HRESULT result;
};

