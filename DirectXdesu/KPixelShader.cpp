#include "KPixelShader.h"

KPixelShader::KPixelShader(){}

KPixelShader::KPixelShader(KDirectInit dx) {
	Init(dx);
	Error(dx);
}

void KPixelShader::Init(KDirectInit dx) {
	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	dx.result = D3DCompileFromFile(
		L"BasicPS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);
}

void KPixelShader::Error(KDirectInit dx) {
	// �G���[���ł���
	if (FAILED(dx.result))
	{
		// erroeBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}