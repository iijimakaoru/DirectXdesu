#include "ParticleManager.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "KInput.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ID3D12Device* Particles::device = nullptr;
ID3D12GraphicsCommandList* Particles::cmdList = nullptr;
std::unique_ptr<KGPlin> Particles::pipeline = nullptr;
KShader Particles::shader;

XMMATRIX Particles::matBillboard = XMMatrixIdentity();
XMMATRIX Particles::matBillboardY = XMMatrixIdentity();

// �r���{�[�h�p�[�e�B�N��
const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	DirectX::XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}

void Particles::Add(int life, KMyMath::Vector3 pos, KMyMath::Vector3 vel, KMyMath::Vector3 accel, float start_scale, float end_scale)
{
	particles.emplace_front();

	Particle& p = particles.front();

	p.pos = pos;
	p.vel = vel;
	p.accel = accel;
	p.num_frame = life;
	p.s_scale = start_scale;
	p.e_scale = end_scale;
}

void Particles::StaticInitialize()
{
	// �f�o�C�X�Z�b�g
	Particles::device = KDirectXCommon::GetInstance()->GetDev();

	// �R�}���h���X�g���Z�b�g
	Particles::cmdList = KDirectXCommon::GetInstance()->GetCmdlist();

	// �p�C�v���C��������
	InitializeGraphicsPipeline();
}

Particles* Particles::Create(TextureData& textureData_)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Particles* object3d = new Particles();
	if (object3d == nullptr)
	{
		return nullptr;
	}

	// ������
	if (!object3d->Initialize(textureData_)) {
		delete object3d;
		assert(0);
		return nullptr;
	}

	return object3d;
}

void Particles::InitializeGraphicsPipeline()
{
	shader.Init(L"Resources/Shader/ParticleVS.hlsl", L"Resources/Shader/ParticlePS.hlsl", "main", L"Resources/Shader/ParticleGS.hlsl");
	pipeline.reset(KGPlin::Create(shader, "Particle"));
}

void Particles::CreateModel()
{
	HRESULT result = S_FALSE;

	// �l�p�`�̃C���f�b�N�X�f�[�^
	unsigned short indicesSquare[] = {
		0,1,2,// �O�p�`1
		2,1,3,// �O�p�`2
	};

	UINT sizeVB = static_cast<UINT>(sizeof(vertices));

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
}

bool Particles::Initialize(TextureData textureData_)
{
	// nullptr�`�F�b�N
	assert(device);

	// �e�N�X�`���f�[�^���
	textureData = textureData_;

	// ���f������
	CreateModel();

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(PConstBufferData) + 0xff) & ~0xff);

	HRESULT result;

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProps, // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	return true;
}

void Particles::Update(ViewProjection* viewProjection)
{
	HRESULT result;

	// �p�[�e�B�N���̍폜
	particles.remove_if([](Particle& p) {return p.frame >= p.num_frame; });

	// �S�p�[�e�B�N���̍X�V
	for (std::forward_list<Particle>::iterator it = particles.begin();
		it != particles.end();
		it++)
	{
		it->frame++;
		it->vel = it->vel + it->accel;
		it->pos = it->pos + it->vel;

		float f = (float)it->frame / it->num_frame;

		it->scale = (it->e_scale - it->s_scale) * f;
		it->scale += it->s_scale;
	}
	// ���_�o�b�t�@�փf�[�^�]��
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		for (std::forward_list<Particle>::iterator it = particles.begin();
			it != particles.end();
			it++)
		{
			vertMap->pos = it->pos;

			vertMap->scale = it->scale;

			vertMap++;
		}
		vertBuff->Unmap(0, nullptr);
	}
	// �萔�o�b�t�@�փf�[�^�]��
	PConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = viewProjection->matView * viewProjection->matProjection;	// �s��̍���
	constMap->matBillboard = MyMathConvert::ChangeXMMATRIXtoMatrix4(matBillboard);	// �s��̍���
	constBuff->Unmap(0, nullptr);
}

void Particles::Draw()
{
	// nullptr�`�F�b�N
	assert(device);
	assert(cmdList);

	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	pipeline->Setting();
	pipeline->Update(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { textureData.srvHeap.Get()};
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, textureData.gpuHandle);

	//�`��R�}���h
	cmdList->DrawInstanced((UINT)std::distance(particles.begin(), particles.end()), 1, 0, 0);
}

ParticleManager* ParticleManager::parthicleManager = nullptr;

void ParticleManager::Init()
{
	// �e�N�X�`���ǂݍ���
	textureData1 = TextureManager::Load("Resources/texture/mario.jpg");
	textureData2 = TextureManager::Load("Resources/texture/kariPlayerColor.png");

	// �p�[�e�B�N��
	particles1.reset(Particles::Create(textureData1));
	particles2.reset(Particles::Create(textureData2));
}

void ParticleManager::Update(ViewProjection* viewPro)
{
	if (KInput::GetInstance()->IsTrigger(DIK_1))
	{
		for (int i = 0; i < 100; i++)
		{
			KMyMath::Vector3 pos{};
			pos.x = 0;
			pos.y = 0;
			pos.z = -150;
			const float md_vel = 0.1f;
			KMyMath::Vector3 vel{};
			vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			KMyMath::Vector3 acc{};
			const float rnd_acc = 0.001f;
			acc.y = (float)rand() / RAND_MAX * rnd_acc;
			particles1->Add(60, pos, vel, acc, 1, 0);
		}
	}

	if (KInput::GetInstance()->IsTrigger(DIK_2))
	{
		for (int i = 0; i < 100; i++)
		{
			const float md_pos = 10.0f;
			KMyMath::Vector3 pos{};
			pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
			pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
			pos.z = ((float)rand() / RAND_MAX * md_pos - md_pos / 2.0f) - 150;
			const float md_vel = 0.1f;
			KMyMath::Vector3 vel{};
			vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			KMyMath::Vector3 acc{};
			const float rnd_acc = 0.001f;
			acc.y = (float)rand() / RAND_MAX * rnd_acc;
			particles2->Add(60, pos, vel, acc, 1, 0);
		}
	}

	particles1->Update(viewPro);
	particles2->Update(viewPro);
}

void ParticleManager::Draw()
{
	particles1->Draw();
	particles2->Draw();
}

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Delete()
{
	delete parthicleManager;
}