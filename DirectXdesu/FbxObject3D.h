#pragma once
#include "FbxModel.h"
#include "Camera.h"
#include "KGPlin.h"

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

	void Update(ViewProjection& viewProjection);

	void Draw();

	void SetPipline(KGPlin* pipeline_)
	{
		FbxObject3D::pipeline = pipeline_;
	}

	void SetModel(FbxModel* model_) 
	{ 
		model = model_; 
	}

	void SetPosition(XMFLOAT3 pos_)
	{
		position = pos_;
	}

	XMFLOAT3 GetPosition()
	{
		return position;
	}

protected:
	ComPtr<ID3D12Resource> constBuffTransform;
	HRESULT result;

	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	// ���[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	// ���f��
	FbxModel* model = nullptr;

	static KGPlin* pipeline;
};

