#pragma once
#include <d3d12.h>
#include <d3dcompiler.h>
#include <wrl.h>

//#pragma comment(lib, "dxgi.lib")

class KShader
{
public:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// Spriteシェーダー
	void SpriteVSNormal();
	void SpriteVSLoadCompile();
	void SpritePSNormal();
	void SpritePSLoadCompile();

	// シェーダー
	KShader() {};
	void Init(LPCWSTR VSFileName, LPCWSTR PSFileName, LPCSTR pEntryPoint = "main", 
		LPCWSTR GSFileName = nullptr, LPCWSTR DSFileName = nullptr, LPCWSTR HSFileName = nullptr);
	// エラー
	void Error();

private:
	// シェーダーオブジェクト
	ComPtr<ID3D10Blob> vsBlob = nullptr; // 頂点シェーダーオブジェクト
	ComPtr<ID3D10Blob> hsBlob = nullptr; // ハルシェーダーオブジェクト
	ComPtr<ID3D10Blob> dsBlob = nullptr; // ドメインシェーダーオブジェクト
	ComPtr<ID3D10Blob> gsBlob = nullptr; // ジオメトリシェーダーオブジェクト
	ComPtr<ID3D10Blob> psBlob = nullptr; // ピクセルシェーダーオブジェクト
	ComPtr<ID3D10Blob> errorBlob = nullptr; // エラーオブジェクト

	// シェーダーバイトコード
	D3D12_SHADER_BYTECODE vsBytecode{}; // 頂点シェーダーバイトコード
	D3D12_SHADER_BYTECODE hsBytecode{}; // ハルシェーダーバイトコード
	D3D12_SHADER_BYTECODE dsBytecode{}; // ドメインシェーダーバイトコード
	D3D12_SHADER_BYTECODE gsBytecode{}; // ジオメトリシェーダーバイトコード
	D3D12_SHADER_BYTECODE psBytecode{}; // ピクセルシェーダーバイトコード

	HRESULT result;

public:
	ID3D10Blob* GetVSBlob() { return vsBlob.Get(); }
	ID3D10Blob* GetHSBlob() { return hsBlob.Get(); }
	ID3D10Blob* GetDSBlob() { return dsBlob.Get(); }
	ID3D10Blob* GetGSBlob() { return gsBlob.Get(); }
	ID3D10Blob* GetPSBlob() { return psBlob.Get(); }

	D3D12_SHADER_BYTECODE* GetVSBytecode() { return &vsBytecode; }
	D3D12_SHADER_BYTECODE* GetHSBytecode() { return &hsBytecode; }
	D3D12_SHADER_BYTECODE* GetDSBytecode() { return &dsBytecode; }
	D3D12_SHADER_BYTECODE* GetGSBytecode() { return &gsBytecode; }
	D3D12_SHADER_BYTECODE* GetPSBytecode() { return &psBytecode; }
};

