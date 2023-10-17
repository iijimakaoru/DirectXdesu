#pragma once
#pragma warning(push)
#pragma warning(disable: 5039)
#pragma warning(disable: 4668)
#pragma warning(disable: 5024)
#pragma warning(disable: 4820)
#pragma warning(disable: 4265)
#pragma warning(disable: 4365)
#pragma warning(disable: 5220)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 4265)
#pragma warning(disable: 5204)
#include <wrl.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <xmemory>
#pragma warning(pop)

#pragma comment(lib, "dxgi.lib")

/**
 * @file KShader.h
 * @brief シェーダー
 * @author 飯島 薫
 */

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
	KShader();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="VSFileName"></param>
	/// <param name="PSFileName"></param>
	/// <param name="pEntryPoint"></param>
	/// <param name="GSFileName"></param>
	/// <param name="DSFileName"></param>
	/// <param name="HSFileName"></param>
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
	// ゲッター
	ID3D10Blob* GetVSBlob();
	ID3D10Blob* GetHSBlob();
	ID3D10Blob* GetDSBlob();
	ID3D10Blob* GetGSBlob();
	ID3D10Blob* GetPSBlob();
	D3D12_SHADER_BYTECODE* GetVSBytecode();
	D3D12_SHADER_BYTECODE* GetHSBytecode();
	D3D12_SHADER_BYTECODE* GetDSBytecode();
	D3D12_SHADER_BYTECODE* GetGSBytecode();
	D3D12_SHADER_BYTECODE* GetPSBytecode();
};

