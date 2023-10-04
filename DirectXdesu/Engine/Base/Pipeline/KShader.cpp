#include "KShader.h"
#include <string>
#include <assert.h>

void KShader::Error() 
{
	// エラーがでたら
	if (FAILED(result)) 
	{
		// erroeBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void KShader::SpritePSNormal()
{
	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shader/SpritePS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);
}

void KShader::SpriteVSNormal()
{
	// 頂点シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shader/SpriteVS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob);
}

void KShader::SpriteVSLoadCompile()
{
	SpriteVSNormal();
	Error();
}

void KShader::SpritePSLoadCompile()
{
	SpritePSNormal();
	Error();
}

void KShader::Init(LPCWSTR VSFileName, LPCWSTR PSFileName, LPCSTR pEntryPoint, LPCWSTR GSFileName, LPCWSTR DSFileName, LPCWSTR HSFileName)
{
#pragma region VertexShader
	//	頂点シェーダファイル読み込み＆コンパイル
	result = D3DCompileFromFile(
		VSFileName,									// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,					// インクルード可能にする
		pEntryPoint, "vs_5_0",									// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	// エラーなら
	Error();

	vsBytecode.pShaderBytecode = vsBlob->GetBufferPointer();
	vsBytecode.BytecodeLength = vsBlob->GetBufferSize();
#pragma endregion

#pragma region HS
	if (HSFileName != nullptr) 
	{
		//	頂点シェーダファイル読み込み＆コンパイル
		result = D3DCompileFromFile(
			HSFileName,									// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,					// インクルード可能にする
			pEntryPoint, "hs_5_0",									// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// デバッグ用設定
			0,
			&hsBlob, &errorBlob);

		// エラーなら
		Error();

		hsBytecode.pShaderBytecode = hsBlob->GetBufferPointer();
		hsBytecode.BytecodeLength = hsBlob->GetBufferSize();
	}
#pragma endregion

#pragma region DS
	if (DSFileName != nullptr) 
	{
		//	頂点シェーダファイル読み込み＆コンパイル
		result = D3DCompileFromFile(
			DSFileName,									// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,					// インクルード可能にする
			pEntryPoint, "ds_5_0",									// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// デバッグ用設定
			0,
			&dsBlob, &errorBlob);

		// エラーなら
		Error();

		dsBytecode.pShaderBytecode = dsBlob->GetBufferPointer();
		dsBytecode.BytecodeLength = dsBlob->GetBufferSize();
	}
#pragma endregion

#pragma region GS
	if (GSFileName != nullptr) 
	{
		//	頂点シェーダファイル読み込み＆コンパイル
		result = D3DCompileFromFile(
			GSFileName,									// シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,					// インクルード可能にする
			pEntryPoint, "gs_5_0",									// エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// デバッグ用設定
			0,
			&gsBlob, &errorBlob);

		// エラーなら
		Error();

		gsBytecode.pShaderBytecode = gsBlob->GetBufferPointer();
		gsBytecode.BytecodeLength = gsBlob->GetBufferSize();
	}
#pragma endregion

#pragma region PixelShader
	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		PSFileName, // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		pEntryPoint, "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	// エラーなら
	Error();

	psBytecode.pShaderBytecode = psBlob->GetBufferPointer();
	psBytecode.BytecodeLength = psBlob->GetBufferSize();
#pragma endregion
}