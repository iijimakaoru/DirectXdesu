#include "KShader.h"
#include <string>
#include <assert.h>

void KShader::Error() {
	// �G���[���ł���
	if (FAILED(result)) {
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

void KShader::SpritePSNormal()
{
	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
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
	// ���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
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

KShader::KShader(LPCWSTR VSFileName, LPCWSTR PSFileName, LPCSTR pEntryPoint)
{
	Init(VSFileName, PSFileName, pEntryPoint);
}

void KShader::Init(LPCWSTR VSFileName, LPCWSTR PSFileName, LPCSTR pEntryPoint)
{
	// ���_�V�F�[�_�[
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

	// �G���[
	Error();

	// �s�N�Z���V�F�[�_�[
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

	// �G���[
	Error();
}