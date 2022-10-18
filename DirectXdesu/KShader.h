#pragma once
#include "KDirectXCommon.h"
#include <d3dcompiler.h>
#include <string>

class KShader
{
public:
	// Basic�V�F�[�_�[
	void BasicVSNormal();
	void BasicVSError();
	void BasicVSLoadCompile();
	void BasicPSNormal();
	void BasicPSError();
	void BasicPSLoadCompile();
	// Sprite�V�F�[�_�[
	void SpriteVSNormal();
	void SpriteVSError();
	void SpriteVSLoadCompile();
	void SpritePSNormal();
	void SpritePSError();
	void SpritePSLoadCompile();

	ID3D10Blob* vsBlob = nullptr; // ���_�V�F�[�_�[�I�u�W�F�N�g
	ID3D10Blob* psBlob = nullptr;
	ID3D10Blob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	HRESULT result;
};

