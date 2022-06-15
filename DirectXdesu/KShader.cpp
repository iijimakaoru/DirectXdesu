#include "KShader.h"

Shader::Shader(){}

Shader::Shader(KDirectInit dx) {
	VertexInit(dx);
	VertexError(dx);
	PixelInit(dx);
	PixelError(dx);
}

void Shader::VertexInit(KDirectInit dx) {
	// 頂点シェーダーの読み込みとコンパイル
	dx.result = D3DCompileFromFile(
		L"BasicVS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob);
}

void Shader::VertexError(KDirectInit dx) {
	// エラーがでたら
	if (FAILED(dx.result)) {
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

void Shader::PixelInit(KDirectInit dx) {
	// ピクセルシェーダの読み込みとコンパイル
	dx.result = D3DCompileFromFile(
		L"BasicPS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);
}

void Shader::PixelError(KDirectInit dx) {
	// エラーがでたら
	if (FAILED(dx.result))
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