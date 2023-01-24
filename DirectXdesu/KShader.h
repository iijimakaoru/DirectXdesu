#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

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

	// シェーダー
	KShader() {};
	KShader(LPCWSTR VSFileName, LPCWSTR PSFileName, LPCSTR pEntryPoint = "main");
	void Init(LPCWSTR VSFileName, LPCWSTR PSFileName, LPCSTR pEntryPoint = "main");
	// エラー
	void Error();

private:
	ComPtr<ID3D10Blob> vsBlob = nullptr; // 頂点シェーダーオブジェクト
	ComPtr<ID3D10Blob> psBlob = nullptr;
	ComPtr<ID3D10Blob> errorBlob = nullptr; // エラーオブジェクト

	HRESULT result;

public:
	ID3D10Blob* GetVSBlob() { return vsBlob.Get(); }
	ID3D10Blob* GetPSBlob() { return psBlob.Get(); }
};

