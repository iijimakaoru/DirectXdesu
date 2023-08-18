#pragma once
#include "KGPlin.h"
#include "KShader.h"
#include "KDirectXCommon.h"
#include "KObject3d.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>

#include <imgui.h>

class Particles
{
public:// �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPos
	{
		KMyMath::Vector3 pos; // xyz���W
		float scale;
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct PConstBufferData
	{
		KMyMath::Matrix4 mat;	// �R�c�ϊ��s��
		KMyMath::Matrix4 matBillboard;
	};

	struct Particle
	{
		KMyMath::Vector3 pos = {};

		KMyMath::Vector3 vel = {};

		KMyMath::Vector3 accel = {};

		int frame = 0;

		int num_frame = 0;

		float scale = 1.0f;

		float s_scale = 1.0f;

		float e_scale = 0.0f;
	};

private: // �萔
	static const int vertexCount = 1024;		// ���_��

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	static void StaticInitialize();

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static Particles* Create(TextureData& textureData_);

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;

	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;

	// �p�C�v���C��
	static std::unique_ptr<KGPlin> pipeline;

	// �V�F�[�_�[
	static KShader shader;

	// �r���{�[�h�s��
	static XMMATRIX matBillboard;
	// Y�����r���{�[�h�s��
	static XMMATRIX matBillboardY;

private:// �ÓI�����o�֐�
	/// <summary>
	/// �O���t�B�b�N�p�C�v���C������
	/// </summary>
	/// <returns>����</returns>
	static void InitializeGraphicsPipeline();

public: // �����o�֐�
	bool Initialize(TextureData textureData_);
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update(ViewProjection* viewProjection);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �p�[�e�B�N���ǉ�
	/// </summary>
	/// <param name="life"></param>
	/// <param name="pos"></param>
	/// <param name="vel"></param>
	/// <param name="accel"></param>
	/// <param name="start_scale"></param>
	/// <param name="end_scale"></param>
	void Add(int life, KMyMath::Vector3 pos, KMyMath::Vector3 vel, KMyMath::Vector3 accel, float start_scale, float end_scale);

	/// <summary>
	/// ���f���쐬
	/// </summary>
	void CreateModel();

private:
	// ���_�f�[�^�z��
	VertexPos vertices[vertexCount];

	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;

private: // �����o�ϐ�
	ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@

	TextureData textureData;

	std::forward_list<Particle> particles;
};

class ParticleManager
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="viewPro"></param>
	void Update(ViewProjection* viewPro);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ����
	/// </summary>
	void CallExp(const KMyMath::Vector3& pos);

	/// <summary>
	/// ������
	/// </summary>
	void CallSmallExp(const KMyMath::Vector3& pos);

private:
	// �C���X�^���X
	static ParticleManager* parthicleManager;

	// �e�N�X�`��
	TextureData textureData1;
	TextureData textureData2;

	// �p�[�e�B�N��
	std::unique_ptr<Particles> particles1;
	std::unique_ptr<Particles> particles2;

public:
	// �C���X�^���X
	static ParticleManager* GetInstance();

	// ���
	void Delete();

private:
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(const ParticleManager&) = default;
	const ParticleManager& operator=(const ParticleManager&) = delete;
};