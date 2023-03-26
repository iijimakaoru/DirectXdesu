#include "ParticleManager.h"
#include "MyMath.h"

#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
const float BillParticleManager::radius = 5.0f;				// ��ʂ̔��a
const float BillParticleManager::prizmHeight = 8.0f;			// ���̍���
ID3D12Device* BillParticleManager::device = nullptr;
UINT BillParticleManager::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* BillParticleManager::cmdList = nullptr;
ComPtr<ID3D12RootSignature> BillParticleManager::rootsignature;
ComPtr<ID3D12PipelineState> BillParticleManager::pipelinestate;
ComPtr<ID3D12DescriptorHeap> BillParticleManager::descHeap;
ComPtr<ID3D12Resource> BillParticleManager::vertBuff;
ComPtr<ID3D12Resource> BillParticleManager::texbuff;
CD3DX12_CPU_DESCRIPTOR_HANDLE BillParticleManager::cpuDescHandleSRV;
CD3DX12_GPU_DESCRIPTOR_HANDLE BillParticleManager::gpuDescHandleSRV;
XMMATRIX BillParticleManager::matView{};
XMMATRIX BillParticleManager::matProjection{};
XMFLOAT3 BillParticleManager::eye = { 0, 0, -5.0f };
XMFLOAT3 BillParticleManager::target = { 0, 0, 0 };
XMFLOAT3 BillParticleManager::up = { 0, 1, 0 };
D3D12_VERTEX_BUFFER_VIEW BillParticleManager::vbView{};
BillParticleManager::VertexPos BillParticleManager::vertices[vertexCount];

XMMATRIX BillParticleManager::matBillboard = XMMatrixIdentity();
XMMATRIX BillParticleManager::matBillboardY = XMMatrixIdentity();

void ParticleManager::CreatePool(KModel* model, KGPlin* pipeline)
{
	for (int i = 0; i < 200; i++)
	{
		std::unique_ptr<Particle> newParticle = std::make_unique<Particle>();
		newParticle->Init(model,pipeline);
		newParticle->SetInfo({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0);

		particles.push_back(std::move(newParticle));

		std::unique_ptr<Particle2> newParticle2 = std::make_unique<Particle2>();
		newParticle2->Init(model, pipeline);
		newParticle2->SetInfo({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, 0);

		particles2.push_back(std::move(newParticle2));
	}
	pooled = true;
}

void ParticleManager::AllDelete()
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		particle->isDead = true;
	}

	for (std::unique_ptr<Particle2>& particle2 : particles2)
	{
		particle2->isDead = true;
	}
}

void ParticleManager::Update(ViewProjection& viewProjection)
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (!particle->IsDead())
		{
			particle->Update(viewProjection);
		}
	}

	for (std::unique_ptr<Particle2>& particle2 : particles2)
	{
		if (!particle2->IsDead())
		{
			particle2->Update(viewProjection);
		}
	}
}

void ParticleManager::Draw()
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (!particle->IsDead())
		{
			particle->Draw();
		}
	}

	for (std::unique_ptr<Particle2>& particle2 : particles2)
	{
		if (!particle2->IsDead())
		{
			particle2->Draw();
		}
	}
}

void ParticleManager::Explosion(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed)
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (particle->IsDead())
		{
			Vector3 velocity =
			{
				MyMath::GetInstance()->GetRand(-speed,speed) * cosf(MyMath::GetInstance()->GetRand(0.0f,4.0f * PI)),
				MyMath::GetInstance()->GetRand(-speed,speed) * sinf(MyMath::GetInstance()->GetRand(0.0f,4.0f * PI)),
				MyMath::GetInstance()->GetRand(-speed,speed) * cosf(MyMath::GetInstance()->GetRand(0.0f,4.0f * PI)),
			};
			particle->SetInfo(pos, velocity, scale, rotation, lifeTimer);

			particle->Revive();

			return;
		}
	}
}

void ParticleManager::Dash(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed, const Vector3& moveVec)
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (particle->IsDead())
		{
			Vector3 velocity =
			{
				MyMath::GetInstance()->GetRand(-speed, speed),
				MyMath::GetInstance()->GetRand(-speed, speed),
				-speed,
			};

			particle->SetInfo(pos, velocity * moveVec, scale, rotation, lifeTimer);

			particle->Revive();

			return;
		}
	}
}

void ParticleManager::DashWave(const Vector3& pos,
	const Vector3& scale,
	const Vector3& rotation,
	const float speed,
	const Vector3& playerVec,
	float angleZ,
	const float lifeTime)
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (particle->IsDead())
		{
			Vector3 velocity =
			{
				speed * cosf(angleZ),
				speed * sinf(angleZ),
				0,
			};

			particle->SetInfo(pos, velocity * playerVec, scale, rotation, lifeTime);

			particle->Revive();

			return;
		}
	}
}

void ParticleManager::RightWave(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed)
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (particle->IsDead())
		{
			Vector3 velocity =
			{
				MyMath::GetInstance()->GetRand(speed / 2, speed),
				speed,
				-speed,
			};

			particle->SetInfo(pos, velocity, scale, rotation, lifeTimer);

			particle->Revive();

			return;
		}

	}
}

void ParticleManager::LeftWave(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed)
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (particle->IsDead())
		{
			Vector3 velocity =
			{
				-MyMath::GetInstance()->GetRand(speed / 2 ,speed),
				speed,
				-speed,
			};

			particle->SetInfo(pos, velocity, scale, rotation, lifeTimer);

			particle->Revive();

			return;
		}

	}
}

void ParticleManager::Splash(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float lifeTimer, const float speed, const Vector3& vec)
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (particle->IsDead())
		{
			Vector3 velocity =
			{
				MyMath::GetInstance()->GetRand(-speed,speed) * vec.x,
				(MyMath::GetInstance()->GetRand(speed / 2, speed) * 4) * vec.y,
				MyMath::GetInstance()->GetRand(-speed,speed) * vec.z,
			};

			particle->SetInfo(pos, velocity, scale, rotation, lifeTimer);

			particle->Revive();

			return;
		}

	}
}

void ParticleManager::TestSplash(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float speed, float angleY, const float lifeTime)
{
	for (std::unique_ptr<Particle>& particle : particles)
	{
		if (particle->IsDead())
		{
			Vector3 velocity =
			{
				speed * cosf(angleY),
				speed * 2,
				speed * sinf(angleY),
			};

			particle->SetInfo(pos, velocity, scale, rotation, lifeTime);

			particle->Revive();

			return;
		}
	}
}

void ParticleManager::Taihun(const Vector3& pos, const Vector3& scale, const Vector3& rotation, const float speed, float angleY, const float lifeTime)
{
	for (std::unique_ptr<Particle2>& particle2 : particles2)
	{
		if (particle2->IsDead())
		{
			Vector3 velocity =
			{
				speed * cosf(angleY),
				speed * sinf(angleY),
				speed * 4,
			};

			particle2->SetInfo(pos, velocity, scale, rotation, lifeTime);

			particle2->Revive();

			return;
		}
	}
}

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::DeleteInstance()
{
	delete ParticleManager::GetInstance();
}

// �r���{�[�h�p�[�e�B�N��
const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}

void BillParticleManager::Add(int life, XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 accel, float start_scale, float end_scale)
{
	particles.emplace_front();

	BillParticle& p = particles.front();

	p.pos = pos;
	p.vel = vel;
	p.accel = accel;
	p.num_frame = life;
	p.s_scale = start_scale;
	p.e_scale = end_scale;
}

void BillParticleManager::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	// nullptr�`�F�b�N
	assert(device);

	BillParticleManager::device = device;

	// �f�X�N���v�^�q�[�v�̏�����
	InitializeDescriptorHeap();

	// �J����������
	InitializeCamera(window_width, window_height);

	// �p�C�v���C��������
	InitializeGraphicsPipeline();

	// �e�N�X�`���ǂݍ���
	//LoadTexture(fileName);

	// ���f������
	CreateModel();

}

void BillParticleManager::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// PreDraw��PostDraw���y�A�ŌĂ΂�Ă��Ȃ���΃G���[
	assert(BillParticleManager::cmdList == nullptr);

	// �R�}���h���X�g���Z�b�g
	BillParticleManager::cmdList = cmdList;

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelinestate.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void BillParticleManager::PostDraw()
{
	// �R�}���h���X�g������
	BillParticleManager::cmdList = nullptr;
}

BillParticleManager* BillParticleManager::Create(const wchar_t* fileName)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	BillParticleManager* object3d = new BillParticleManager();
	if (object3d == nullptr) {
		return nullptr;
	}

	// ������
	if (!object3d->Initialize(fileName)) {
		delete object3d;
		assert(0);
		return nullptr;
	}

	return object3d;
}

void BillParticleManager::SetEye(XMFLOAT3 eye)
{
	BillParticleManager::eye = eye;

	UpdateViewMatrix();
}

void BillParticleManager::SetTarget(XMFLOAT3 target)
{
	BillParticleManager::target = target;

	UpdateViewMatrix();
}

void BillParticleManager::CameraMoveVector(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void BillParticleManager::CameraMoveEyeVector(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	eye_moved.z = -20;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void BillParticleManager::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v�𐶐�	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NumDescriptors = 1; // �V�F�[�_�[���\�[�X�r���[1��
	result = KDirectXCommon::GetInstance()->GetDev()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//����
	if (FAILED(result)) {
		assert(0);
	}

	// �f�X�N���v�^�T�C�Y���擾
	descriptorHandleIncrementSize = KDirectXCommon::GetInstance()->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

}

void BillParticleManager::InitializeCamera(int window_width, int window_height)
{
	// �r���[�s��̐���
	UpdateViewMatrix();

	// ���s���e�ɂ��ˉe�s��̐���
	//constMap->mat = XMMatrixOrthographicOffCenterLH(
	//	0, window_width,
	//	window_height, 0,
	//	0, 1);
	// �������e�ɂ��ˉe�s��̐���
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f
	);
}

void BillParticleManager::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> gsBlob; // �W�I���g���V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shader/ParticleVS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shader/ParticleGS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "gs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&gsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shader/ParticlePS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",0,DXGI_FORMAT_R32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	/*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;*/

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	//gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1;	// �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = KDirectXCommon::GetInstance()->GetDev()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = rootsignature.Get();

	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = KDirectXCommon::GetInstance()->GetDev()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
	assert(SUCCEEDED(result));

}

void BillParticleManager::LoadTexture(const wchar_t* fileName)
{
	HRESULT result = S_FALSE;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(fileName, WIC_FLAGS_NONE, &metadata, scratchImg);
	assert(SUCCEEDED(result));

	ScratchImage mipChain{};
	// �~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format, metadata.width, (UINT)metadata.height, (UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	// �e�N�X�`���p�o�b�t�@�̐���
	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr, IID_PPV_ARGS(&texbuff));
	assert(SUCCEEDED(result));

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		const Image* img = scratchImg.GetImage(i, 0, 0); // ���f�[�^���o
		result = texbuff->WriteToSubresource(
			(UINT)i,
			nullptr,              // �S�̈�փR�s�[
			img->pixels,          // ���f�[�^�A�h���X
			(UINT)img->rowPitch,  // 1���C���T�C�Y
			(UINT)img->slicePitch // 1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	// �V�F�[�_���\�[�X�r���[�쐬
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	KDirectXCommon::GetInstance()->GetDev()->CreateShaderResourceView(texbuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		cpuDescHandleSRV
	);

}

void BillParticleManager::CreateModel()
{
	HRESULT result = S_FALSE;

	std::vector<VertexPos> realVertices;

	/*VertexPos verticesPoint[] =
	{
		{{0.0f,0.0f,0.0f}},
	};
	std::copy(std::begin(verticesPoint), std::end(verticesPoint), vertices);*/
	/*for (int i = 0; i < vertexCount; i++)
	{
		const float rnd_width = 10.0f;
		vertices[i].pos.x = (float)rand() / RAND_MAX * rnd_width - rnd_width / 2.0f;
		vertices[i].pos.y = (float)rand() / RAND_MAX * rnd_width - rnd_width / 2.0f;
		vertices[i].pos.z = (float)rand() / RAND_MAX * rnd_width - rnd_width / 2.0f;
	}*/
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
	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
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

void BillParticleManager::UpdateViewMatrix()
{
	// �r���[�s��̍X�V
	// ���_���W
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	// �����X���W
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	// �����
	XMVECTOR upVector = XMLoadFloat3(&up);
	// �J����Z��
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	// 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	// �x�N�g���𐳋K��
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);
	// �J������X��
	XMVECTOR cameraAxisX;
	// X���͏������Z���̊O�ςŌ��܂�
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	// �x�N�g���𐳋K��
	cameraAxisX = XMVector3Normalize(cameraAxisX);
	// �J������Y��
	XMVECTOR cameraAxisY;
	// Y����Z����X���̊O�ςŌ��܂�
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	// �J������]�s��
	XMMATRIX matCameraRot;
	// �J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	// �z�u�ɂ��t�s��(�t��])���v�Z
	matView = XMMatrixTranspose(matCameraRot);
	// ���_���W��-1���|�������W
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	// �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g��
	XMVECTOR tX = XMVector3Dot(matCameraRot.r[0], reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(matCameraRot.r[1], reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(matCameraRot.r[2], reverseEyePosition);
	// ��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	// �r���[�s��ɕ��s�ړ�������ݒ�
	matView.r[3] = translation;
#pragma region �S�����r���{�[�h�s��̌v�Z
	// �r���{�[�h�s��
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion

#pragma region Y�����r���{�[�h�s��v�Z
	// 
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	// 
	ybillCameraAxisX = cameraAxisX;
	// 
	ybillCameraAxisY = XMVector3Normalize(upVector);
	// 
	ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);
	// 
	matBillboardY.r[0] = ybillCameraAxisX;
	matBillboardY.r[1] = ybillCameraAxisY;
	matBillboardY.r[2] = ybillCameraAxisZ;
	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion
}

bool BillParticleManager::Initialize(const wchar_t* fileName)
{
	// nullptr�`�F�b�N
	assert(KDirectXCommon::GetInstance()->GetDev());

	LoadTexture(fileName);

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(PConstBufferData) + 0xff) & ~0xff);

	HRESULT result;

	// �萔�o�b�t�@�̐���
	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
		&heapProps, // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	return true;
}

void BillParticleManager::Update(ViewProjection& viewProjection)
{
	HRESULT result;
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	//matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	/*matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);*/

	// ���[���h�s��̍���
	//matWorld = XMMatrixIdentity(); // �ό`�����Z�b�g

	//if (isBillboard)
	//{
	//	if (isBillboardY)
	//	{
	//		matWorld *= matBillboardY; // �r���{�[�h�s����|����
	//	}
	//	else
	//	{
	//		matWorld *= matBillboard; // �r���{�[�h�s����|����
	//	}
	//}

	//matWorld *= matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	//matWorld *= matRot; // ���[���h�s��ɉ�]�𔽉f
	//matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	// �e�I�u�W�F�N�g�������
	//if (parent != nullptr) {
	//	// �e�I�u�W�F�N�g�̃��[���h�s����|����
	//	matWorld *= parent->matWorld;
	//}

	SetEye(viewProjection.eye);
	SetTarget(viewProjection.target);

	// �p�[�e�B�N���̍폜
	particles.remove_if([](BillParticle& p) {return p.frame >= p.num_frame; });
	// �S�p�[�e�B�N���̍X�V
	for (std::forward_list<BillParticle>::iterator it = particles.begin();
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
		for (std::forward_list<BillParticle>::iterator it = particles.begin();
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
	constMap->mat = viewProjection.matView * viewProjection.matProjection;	// �s��̍���
	constMap->matBillboard = matBillboard;	// �s��̍���
	constBuff->Unmap(0, nullptr);
}

void BillParticleManager::Draw()
{
	// nullptr�`�F�b�N
	assert(device);
	assert(BillParticleManager::cmdList);

	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	//cmdList->IASetIndexBuffer(&ibView);

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);
	// �`��R�}���h
	//cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);
	cmdList->DrawInstanced((UINT)std::distance(particles.begin(), particles.end()), 1, 0, 0);
}