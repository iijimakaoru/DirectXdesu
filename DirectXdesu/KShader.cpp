#include "KShader.h"
#include <string>
#include <assert.h>

void KShader::BasicPSNormal() {
	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicPS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);
}

void KShader::Error() {
	// エラーがでたら
	if (FAILED(result)) {
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

void KShader::BasicPSLoadCompile()
{
	BasicPSNormal();
	Error();
}

void KShader::BasicVSNormal() {
	// 頂点シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicVS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob);
}

void KShader::BasicVSLoadCompile()
{
	BasicVSNormal();
	Error();
}

void KShader::SpritePSNormal()
{
	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"SpritePS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);
}

void KShader::SpritePSLoadCompile()
{
	SpritePSNormal();
	Error();
}

void KShader::ObjVSNormal()
{
	result = D3DCompileFromFile(
		L"ObjVS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob);
}

void KShader::ObjVSLoadCompile()
{
	ObjVSNormal();
	Error();
}

void KShader::ObjPSNormal()
{
	result = D3DCompileFromFile(
		L"ObjPS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);
}

void KShader::ObjPSLoadCompile()
{
	ObjPSNormal();
	Error();
}

KShader::KShader(LPCWSTR VSFileName, LPCWSTR PSFileName, LPCSTR pEntryPoint)
{
	Init(VSFileName, PSFileName, pEntryPoint);
}

void KShader::Init(LPCWSTR VSFileName, LPCWSTR PSFileName, LPCSTR pEntryPoint)
{
	// 頂点シェーダー
	result = D3DCompileFromFile(
		VSFileName,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		pEntryPoint,
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob,
		&errorBlob
	);

	// エラー
	Error();

	// ピクセルシェーダー
	result = D3DCompileFromFile(
		PSFileName,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		pEntryPoint,
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob,
		&errorBlob
	);

	// エラー
	Error();
}

void KShader::SpriteVSNormal()
{
	// 頂点シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"SpriteVS.hlsl",
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