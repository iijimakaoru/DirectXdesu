#pragma once
#include "FbxModel.h"
#include "Camera.h"

class FbxObject3D
{
protected: // �G�C���A�X
	// �ȗ�����
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �ÓI�����o�֐�
	// �Z�b�^�[
	
public: // �G�C���A�X
	// �萔�o�b�t�@�f�[�^�\����
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj; // �r���[�v���W�F�N�V����
		XMMATRIX world;	   // ���[���h�s��
		XMFLOAT3 cameraPos;// �J�������W
	};

public:
	void Init();

protected:
	ComPtr<ID3D12Resource> constBuffTransform;
	HRESULT result;
};

