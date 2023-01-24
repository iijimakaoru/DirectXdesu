#pragma once
#include "KDirectXCommon.h"
#include <d3dcompiler.h>
#include <string>

class KShader
{
public:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

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

	ComPtr<ID3D10Blob> vsBlob = nullptr; // 頂点シェーダーオブジェクト
	ComPtr<ID3D10Blob> psBlob = nullptr;
	ComPtr<ID3D10Blob> errorBlob = nullptr; // エラーオブジェクト

	HRESULT result;
};

