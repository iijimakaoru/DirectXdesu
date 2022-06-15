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
	ID3D10Blob* vsBlob = nullptr; // 頂点シェーダーオブジェクト
	ID3D10Blob* psBlob = nullptr; // ピクセルシェーダーオブジェクト
	ID3D10Blob* errorBlob = nullptr; // エラーオブジェクト
};

