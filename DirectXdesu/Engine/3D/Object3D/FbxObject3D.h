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
	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="viewProjection"></param>
	void Update(ViewProjection& viewProjection);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �p�C�v���C���Z�b�^�[
	/// </summary>
	/// <param name="pipeline_"></param>
	void SetPipline(KGPlin* pipeline_)
	{
		FbxObject3D::pipeline = pipeline_;
	}

	/// <summary>
	/// ���f���Z�b�^�[
	/// </summary>
	/// <param name="model_"></param>
	void SetModel(FbxModel* model_) 
	{ 
		model = model_; 
	}

	/// <summary>
	/// �|�W�V�����Z�b�^�[
	/// </summary>
	/// <param name="pos_"></param>
	void SetPosition(XMFLOAT3 pos_)
	{
		position = pos_;
	}

	/// <summary>
	/// �|�W�V�����Q�b�^�[
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetPosition()
	{
		return position;
	}

	/// <summary>
	/// �A�j���[�V�����J�n
	/// </summary>
	void PlayAnimation();

public:// �萔
	// �{�[���̍ő�l
	static const int MAX_BONES = 32;

	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

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

	ComPtr<ID3D12Resource> constBuffSkin;

	// 1�t���[���̎���
	FbxTime frameTime;
	// �A�j���[�V�����J�n����
	FbxTime startTime;
	// �A�j���[�V�����I������
	FbxTime endTime;
	// �A�j���[�V�������ݎ���
	FbxTime currentTime;
	// �A�j���[�V�����Đ��t���O
	bool isPlay = false;

	static KGPlin* pipeline;
};

