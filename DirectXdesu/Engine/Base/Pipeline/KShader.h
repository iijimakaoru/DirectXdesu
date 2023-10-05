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

class KShader
{
public:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// Sprite�V�F�[�_�[
	void SpriteVSNormal();
	void SpriteVSLoadCompile();
	void SpritePSNormal();
	void SpritePSLoadCompile();

	// �V�F�[�_�[
	KShader();
	void Init(LPCWSTR VSFileName, LPCWSTR PSFileName, LPCSTR pEntryPoint = "main", 
		LPCWSTR GSFileName = nullptr, LPCWSTR DSFileName = nullptr, LPCWSTR HSFileName = nullptr);
	// �G���[
	void Error();

private:
	// �V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3D10Blob> vsBlob = nullptr; // ���_�V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3D10Blob> hsBlob = nullptr; // �n���V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3D10Blob> dsBlob = nullptr; // �h���C���V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3D10Blob> gsBlob = nullptr; // �W�I���g���V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3D10Blob> psBlob = nullptr; // �s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3D10Blob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	// �V�F�[�_�[�o�C�g�R�[�h
	D3D12_SHADER_BYTECODE vsBytecode{}; // ���_�V�F�[�_�[�o�C�g�R�[�h
	D3D12_SHADER_BYTECODE hsBytecode{}; // �n���V�F�[�_�[�o�C�g�R�[�h
	D3D12_SHADER_BYTECODE dsBytecode{}; // �h���C���V�F�[�_�[�o�C�g�R�[�h
	D3D12_SHADER_BYTECODE gsBytecode{}; // �W�I���g���V�F�[�_�[�o�C�g�R�[�h
	D3D12_SHADER_BYTECODE psBytecode{}; // �s�N�Z���V�F�[�_�[�o�C�g�R�[�h

	HRESULT result;

public:
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

