#pragma once
#include "Particle.h"
#include <list>
#include <memory>
#include "KGPlin.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>

class ParticleManager
{
public:
	bool IsPoolCreated() {
		return pooled;
	}
	void CreatePool(KModel* model, KGPlin* pipeline);
	void AllDelete();
	void Update(ViewProjection& viewProjection);
	void Draw();
	void Explosion(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed);
	void Dash(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed, const Vector3& moveVec);
	void DashWave(const Vector3& pos,
		const Vector3& scale,
		const Vector3& rotation,
		const float speed,
		const Vector3& playerVec,
		float angleZ,
		const float lifeTime);
	void RightWave(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed);
	void LeftWave(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed);
	void Splash(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed, const Vector3& vec);
	void TestSplash(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float speed, float angleY, const float lifeTime);
	void Taihun(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float speed, float angleY, const float lifeTime);

private:
	std::list<std::unique_ptr<Particle>> particles;
	std::list<std::unique_ptr<Particle2>> particles2;
	bool pooled = false;

public:
	static ParticleManager* GetInstance();
	static void DeleteInstance();

private:
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(const ParticleManager&) = delete;
	const ParticleManager& operator=(const ParticleManager&) = delete;
};

class BillParticleManager
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:// �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz���W
		float scale;
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct PConstBufferData
	{
		XMMATRIX mat;	// �R�c�ϊ��s��
		XMMATRIX matBillboard;
	};

	struct BillParticle
	{
		using XMFLOAT3 = DirectX::XMFLOAT3;

		XMFLOAT3 pos = {};

		XMFLOAT3 vel = {};

		XMFLOAT3 accel = {};

		int frame = 0;

		int num_frame = 0;

		float scale = 1.0f;

		float s_scale = 1.0f;

		float e_scale = 0.0f;
	};

private: // �萔
	static const int division = 50;					// ������
	static const float radius;				// ��ʂ̔��a
	static const float prizmHeight;			// ���̍���
	static const int planeCount = division * 2 + division * 2;		// �ʂ̐�
	static const int vertexCount = 1024;		// ���_��

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	/// <summary>
	/// �`��O����
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static BillParticleManager* Create(const wchar_t* fileName);

	/// <summary>
	/// ���_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	static const XMFLOAT3& GetEye() { return eye; }

	/// <summary>
	/// ���_���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	static void SetEye(XMFLOAT3 eye);

	/// <summary>
	/// �����_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	static const XMFLOAT3& GetTarget() { return target; }

	/// <summary>
	/// �����_���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	static void SetTarget(XMFLOAT3 target);

	/// <summary>
	/// �x�N�g���ɂ��ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	static void CameraMoveVector(XMFLOAT3 move);

	/// <summary>
	/// �x�N�g���ɂ�鎋�_�ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	static void CameraMoveEyeVector(XMFLOAT3 move);

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// ���_�o�b�t�@
	static ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	//static ComPtr<ID3D12Resource> indexBuff;
	// �e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texbuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �r���[�s��
	static XMMATRIX matView;
	// �ˉe�s��
	static XMMATRIX matProjection;
	// ���_���W
	static XMFLOAT3 eye;
	// �����_���W
	static XMFLOAT3 target;
	// ������x�N�g��
	static XMFLOAT3 up;
	// ���_�o�b�t�@�r���[
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// ���_�f�[�^�z��
	static VertexPos vertices[vertexCount];
	// �r���{�[�h�s��
	static XMMATRIX matBillboard;
	// Y�����r���{�[�h�s��
	static XMMATRIX matBillboardY;

private:// �ÓI�����o�֐�
	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	static void InitializeDescriptorHeap();

	/// <summary>
	/// �J����������
	/// </summary>
	/// <param name="window_width">��ʉ���</param>
	/// <param name="window_height">��ʏc��</param>
	static void InitializeCamera(int window_width, int window_height);

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C������
	/// </summary>
	/// <returns>����</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	static void LoadTexture(const wchar_t* fileName);

	/// <summary>
	/// ���f���쐬
	/// </summary>
	static void CreateModel();

	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	static void UpdateViewMatrix();

public: // �����o�֐�
	bool Initialize(const wchar_t* fileName);
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update(ViewProjection& viewProjection);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	void Add(int life, XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 accel, float start_scale, float end_scale);

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	//const XMFLOAT3& GetPosition() const { return position; }

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	//void SetPosition(const XMFLOAT3& position) { this->position = position; }

	void IsBillboard() { isBillboard = true; }

	bool isBillboard = false;

	bool isBillboardY = false;

	bool SpawnParticle()
	{
		if (particles.max_size() > 100)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

private: // �����o�ϐ�
	ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@
	// �F
	//XMFLOAT4 color = { 1,1,1,1 };
	// ���[�J���X�P�[��
	//XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	//XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	//XMFLOAT3 position = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	//MMATRIX matWorld;
	// �e�I�u�W�F�N�g
	//ParticleManager* parent = nullptr;

	std::forward_list<BillParticle> particles;
};