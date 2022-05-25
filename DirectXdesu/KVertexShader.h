#pragma once
#include "KDirectInit.h"
#include <d3dcompiler.h>

class KVertexShader
{
public:
	KVertexShader(KDirectInit dx);
	void Init(KDirectInit dx);
	void Error(KDirectInit dx);
	ID3D10Blob* vsBlob = nullptr; // 頂点シェーダーオブジェクト
	ID3D10Blob* errorBlob = nullptr; // エラーオブジェクト
};

