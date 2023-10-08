#include "ParticleManager.h"
#include "KInput.h"

#pragma warning(push)
#pragma warning(disable: 4820)
#include <d3dcompiler.h>
#pragma warning(pop)

#pragma comment(lib, "d3dcompiler.lib")

#include "Ease.h"

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* Particles::device = nullptr;
ID3D12GraphicsCommandList* Particles::cmdList = nullptr;
std::unique_ptr<KGPlin> Particles::pipeline = nullptr;
KShader Particles::shader;

DirectX::XMMATRIX Particles::matBillboard = DirectX::XMMatrixIdentity();
DirectX::XMMATRIX Particles::matBillboardY = DirectX::XMMatrixIdentity();

// ビルボードパーティクル
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
	// デバイスセット
	Particles::device = KDirectXCommon::GetInstance()->GetDev();

	// コマンドリストをセット
	Particles::cmdList = KDirectXCommon::GetInstance()->GetCmdlist();

	// パイプライン初期化
	InitializeGraphicsPipeline();
}

Particles* Particles::Create(TextureData& textureData_)
{
	// 3Dオブジェクトのインスタンスを生成
	Particles* object3d = new Particles();
	if (object3d == nullptr)
	{
		return nullptr;
	}

	// 初期化
	if (!object3d->Initialize(textureData_)) {
		delete object3d;
		assert(0);
		return nullptr;
	}

	return object3d;
}

void Particles::InitializeGraphicsPipeline()
{
	shader.Init(
		L"Resources/Shader/ParticleVS.hlsl", 
		L"Resources/Shader/ParticlePS.hlsl", 
		"main", 
		L"Resources/Shader/ParticleGS.hlsl"
	);

	pipeline.reset(KGPlin::Create(shader, "Particle"));
}

void Particles::CreateModel()
{
	HRESULT result = S_FALSE;

	// 四角形のインデックスデータ
	unsigned short indicesSquare[] = {
		0,1,2,// 三角形1
		2,1,3,// 三角形2
	};

	UINT sizeVB = static_cast<UINT>(sizeof(vertices));

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// 頂点バッファへのデータ転送
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);
}

bool Particles::Initialize(TextureData textureData_)
{
	// nullptrチェック
	assert(device);

	// テクスチャデータ代入
	textureData = textureData_;

	// モデル生成
	CreateModel();

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(PConstBufferData) + 0xff) & ~0xff);

	HRESULT result;

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	return true;
}

void Particles::Update(ViewProjection* viewProjection)
{
	HRESULT result;

	// パーティクルの削除
	particles.remove_if([](Particle& p) {return p.frame >= p.num_frame; });

	// 全パーティクルの更新
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
	// 頂点バッファへデータ転送
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
	// 定数バッファへデータ転送
	PConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = viewProjection->GetMatView() * viewProjection->GetMatPro();	// 行列の合成
	constMap->matBillboard = MyMathConvert::ChangeXMMATRIXtoMatrix4(matBillboard);	// 行列の合成
	constBuff->Unmap(0, nullptr);
}

void Particles::Draw()
{
	// nullptrチェック
	assert(device);
	assert(cmdList);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	pipeline->Setting();
	pipeline->Update(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { textureData.srvHeap.Get()};
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, textureData.gpuHandle);

	//描画コマンド
	cmdList->DrawInstanced((UINT)std::distance(particles.begin(), particles.end()), 1, 0, 0);
}

ParticleManager* ParticleManager::parthicleManager = nullptr;

void ParticleManager::Init()
{
	// テクスチャ読み込み
	textureData1 = TextureManager::Load("Resources/texture/mario.jpg");
	textureData2 = TextureManager::Load("Resources/texture/kariPlayerColor.png");

	// パーティクル
	particles1.reset(Particles::Create(textureData1));
	particles2.reset(Particles::Create(textureData2));
}

void ParticleManager::Update(ViewProjection* viewPro)
{
	particles1->Update(viewPro);
	particles2->Update(viewPro);
}

void ParticleManager::Draw()
{
	particles1->Draw();
	particles2->Draw();
}

void ParticleManager::CallExp(const KMyMath::Vector3& pos)
{
	for (int i = 0; i < 100; i++)
	{
		const float md_vel = 1.0f;
		KMyMath::Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;

		KMyMath::Vector3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		particles2->Add(30, pos, vel, acc, 2, 0);
	}
}

void ParticleManager::CallSmallExp(const KMyMath::Vector3& pos)
{
	for (int i = 0; i < 20; i++)
	{
		const float md_vel = 1.0f;
		KMyMath::Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;

		KMyMath::Vector3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		particles2->Add(10, pos, vel, acc, 2, 0);
	}
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

ObjParticle* ObjParticle::Create(const KMyMath::Vector3& pos_, 
	KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& velocity_, TextureData& tex)
{
	// インスタンス
	ObjParticle* instance = new ObjParticle();
	if (instance == nullptr)
	{
		return nullptr;
	}

	// 初期化
	instance->Init(pos_, model_, pipeline_, velocity_, tex);

	return instance;
}

void ObjParticle::Init(const KMyMath::Vector3& pos_, 
	KModel* model_, KGPlin* pipeline_, const KMyMath::Vector3& velocity_, TextureData& tex)
{
	object3d.reset(KObject3d::Create(model_, pipeline_));

	object3d->transform.pos = pos_;

	object3d->transform.scale = { 0.5f,0.5f,0.5f };

	velocity = velocity_;

	lifeTimer = 0;

	texture = tex;
}

void ObjParticle::Update(ViewProjection* viewPro)
{
	if (lifeTimer < lifeTime)
	{
		lifeTimer++;

		object3d->transform.pos += velocity;

		object3d->transform.rot += {30, 30, 30};

		if (lifeTimer > 40)
		{
			if (easeTimer < easeTime)
			{
				easeTimer++;
				object3d->transform.scale = MyEase::OutQuadVec3({ 0.5f,0.5f,0.5f }, { 0,0,0 }, easeTimer / easeTime);
			}
		}
	}
	else
	{
		isDead = true;
	}

	object3d->Update(viewPro);
}

void ObjParticle::Draw()
{
	object3d->Draw(texture);
}

ObjParticleManager* ObjParticleManager::objParticleManager = nullptr;

void ObjParticleManager::Init()
{
	// Obj
	shader.Init(L"Resources/Shader/ObjVS.hlsl", L"Resources/Shader/ObjPS.hlsl");
	pipeline.reset(KGPlin::Create(shader, "Obj"));

	// キューブ生成
	model = std::make_unique<Cube>();
	model->CreateModel();

	// テクスチャ
	textureData1 = TextureManager::Load("Resources/texture/kariPlayerColor.png");
}

void ObjParticleManager::Update(ViewProjection* viewPro)
{
	objParticles.remove_if([](std::unique_ptr<ObjParticle>& objParticle) {return objParticle->GetIsDead(); });

	for (std::unique_ptr<ObjParticle>& objParticle : objParticles)
	{
		objParticle->Update(viewPro);
	}
}

void ObjParticleManager::Draw()
{
	for (std::unique_ptr<ObjParticle>& objParticle : objParticles)
	{
		objParticle->Draw();
	}
}

void ObjParticleManager::SetExp(const KMyMath::Vector3& pos_)
{
	std::unique_ptr<ObjParticle> newParticle;
	for (size_t i = 0; i < 40; i++)
	{
		// 生成
		newParticle.reset(ObjParticle::Create(pos_,
			model.get(),
			pipeline.get(),
			{ MyMathUtility::GetRand(-1.0f,1.0f),MyMathUtility::GetRand(-1.0f,1.0f),MyMathUtility::GetRand(-1.0f,1.0f) }, 
			textureData1));
		// 出力
		objParticles.push_back(std::move(newParticle));
	}
}

void ObjParticleManager::SetSmallExp(const KMyMath::Vector3& pos_)
{
	std::unique_ptr<ObjParticle> newParticle;
	for (size_t i = 0; i < 10; i++)
	{
		// 生成
		newParticle.reset(ObjParticle::Create(pos_,
			model.get(),
			pipeline.get(),
			{ MyMathUtility::GetRand(-0.25f,0.25f),MyMathUtility::GetRand(-0.25f,0.25f),MyMathUtility::GetRand(-0.25f,0.25f) },
			textureData1));
		// 出力
		objParticles.push_back(std::move(newParticle));
	}
}

ObjParticleManager* ObjParticleManager::GetInstance()
{
	static ObjParticleManager instance;
	return &instance;
}

void ObjParticleManager::Delete()
{
	delete objParticleManager;
}
