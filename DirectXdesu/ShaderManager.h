#pragma once
#include "KDirectXCommon.h"
#include <d3dcompiler.h>

class ShaderManager
{
public:
	virtual void Init(){}
	virtual void Error(){}
};

class PS : public ShaderManager
{
public:
	void Init() override;
	void Error() override;

	ID3D10Blob* normalBlob = nullptr;
	ID3D10Blob* errorBlob = nullptr;

	HRESULT result;
};

class VS : public ShaderManager
{
public:
	void Init() override;
	void Error() override;

	ID3D10Blob* normalBlob = nullptr;
	ID3D10Blob* errorBlob = nullptr;

	HRESULT result;
};