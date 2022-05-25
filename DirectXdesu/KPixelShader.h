#pragma once
#include "KDirectInit.h"
#include <d3dcompiler.h>

class KPixelShader
{
public:
	KPixelShader(KDirectInit dx);
	void Init(KDirectInit dx);
	void Error(KDirectInit dx);
	ID3D10Blob* psBlob = nullptr; // ピクセルシェーダーオブジェクト
	ID3D10Blob* errorBlob = nullptr; // エラーオブジェクト
};

