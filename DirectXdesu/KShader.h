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

	ID3D10Blob* vsBlob = nullptr; // 頂点シェーダーオブジェクト
	ID3D10Blob* psBlob = nullptr;
	ID3D10Blob* errorBlob = nullptr; // エラーオブジェクト

	HRESULT result;
};

