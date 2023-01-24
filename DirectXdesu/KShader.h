#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class KShader
{
public:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// Basic�V�F�[�_�[
	void BasicVSNormal();
	void BasicVSLoadCompile();
	void BasicPSNormal();
	void BasicPSLoadCompile();
	// Sprite�V�F�[�_�[
	void SpriteVSNormal();
	void SpriteVSLoadCompile();
	void SpritePSNormal();
	void SpritePSLoadCompile();
	// Obj�V�F�[�_�[
	void ObjVSNormal();
	void ObjVSLoadCompile();
	void ObjPSNormal();
	void ObjPSLoadCompile();

	// �V�F�[�_�[
	KShader() {};
	KShader(LPCWSTR VSFileName, LPCWSTR PSFileName, LPCSTR pEntryPoint = "main");
	void Init(LPCWSTR VSFileName, LPCWSTR PSFileName, LPCSTR pEntryPoint = "main");
	// �G���[
	void Error();

private:
	ComPtr<ID3D10Blob> vsBlob = nullptr; // ���_�V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3D10Blob> psBlob = nullptr;
	ComPtr<ID3D10Blob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	HRESULT result;

public:
	ID3D10Blob* GetVSBlob() { return vsBlob.Get(); }
	ID3D10Blob* GetPSBlob() { return psBlob.Get(); }
};

