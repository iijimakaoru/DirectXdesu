#pragma once
#include <d3d12.h>
#include <d3dcompiler.h>
#include <wrl.h>

//#pragma comment(lib, "dxgi.lib")

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
	KShader() {};
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

