#pragma once
#include "KDirectXCommon.h"
#include <d3dcompiler.h>
#include <string>

class KShader
{
public:
	// Basicシェーダー
	void BasicVSNormal();
	void BasicVSError();
	void BasicVSLoadCompile();
	void BasicPSNormal();
	void BasicPSError();
	void BasicPSLoadCompile();
	// Spriteシェーダー
	void SpriteVSNormal();
	void SpriteVSError();
	void SpriteVSLoadCompile();
	void SpritePSNormal();
	void SpritePSError();
	void SpritePSLoadCompile();

	ID3D10Blob* vsBlob = nullptr; // 頂点シェーダーオブジェクト
	ID3D10Blob* psBlob = nullptr;
	ID3D10Blob* errorBlob = nullptr; // エラーオブジェクト

	HRESULT result;
};

