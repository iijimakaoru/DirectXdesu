#pragma once
#include "KDirectXCommon.h"
#include <d3dcompiler.h>
#include <string>

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
	// �G���[
	void Error();

	ComPtr<ID3D10Blob> vsBlob = nullptr; // ���_�V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3D10Blob> psBlob = nullptr;
	ComPtr<ID3D10Blob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	HRESULT result;
};

