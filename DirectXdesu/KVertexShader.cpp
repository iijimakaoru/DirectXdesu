#include "KVertexShader.h"

KVertexShader::KVertexShader(KDirectInit dx) {
	Init(dx);
	Error(dx);
}

void KVertexShader::Init(KDirectInit dx) {
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

void KVertexShader::Error(KDirectInit dx) {
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