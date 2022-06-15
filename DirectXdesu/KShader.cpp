#include "KShader.h"

Shader::Shader(){}

Shader::Shader(KDirectInit dx) {
	VertexInit(dx);
	VertexError(dx);
	PixelInit(dx);
	PixelError(dx);
}

void Shader::VertexInit(KDirectInit dx) {
	// ���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
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
	// �G���[���ł���
	if (FAILED(dx.result)) {
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

void Shader::PixelInit(KDirectInit dx) {
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

void Shader::PixelError(KDirectInit dx) {
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