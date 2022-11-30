#pragma once
#include "KDirectXCommon.h"
#include <d3dcompiler.h>
#include <string>

class KShader
{
public:
	// Basicシェーダー
	void BasicVSNormal();
	void BasicVSLoadCompile();
	void BasicPSNormal();
	void BasicPSLoadCompile();
	// Spriteシェーダー
	void SpriteVSNormal();
	void SpriteVSLoadCompile();
	void SpritePSNormal();
	void SpritePSLoadCompile();
	// Objシェーダー
	void ObjVSNormal();
	void ObjVSLoadCompile();
	void ObjPSNormal();
	void ObjPSLoadCompile();
	// エラー
	void Error();

	ID3D10Blob* vsBlob = nullptr; // 頂点シェーダーオブジェクト
	ID3D10Blob* psBlob = nullptr;
	ID3D10Blob* errorBlob = nullptr; // エラーオブジェクト

	HRESULT result;
};

