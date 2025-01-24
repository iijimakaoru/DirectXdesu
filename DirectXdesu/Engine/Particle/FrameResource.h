#pragma once
#include "d3dUtil.h"
#include "MathHelper.h"
#include "UploadBuffer.h"
#include "KVertex.h"

struct ObjectConstants
{
	DirectX::XMFLOAT4X4 World = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 View = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 Projection = MathHelper::Identity4x4();
	float AspectRatio = 0.0f;
};

struct TimeConstants
{
	float DeltaTime = 0.0f;
	float TotalTime = 0.0f;
};

struct ParticleConstants
{
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 velocity;
	float LifeTime = 0.0f;
	DirectX::XMFLOAT3 acceleration;
	float pad;
	int EmitCount = 0;
	int MaxParticles = 0;
	int GridSize = 0;
	uint32_t vertexNum = 0;
};

struct FrameResource
{
public:

	FrameResource(ID3D12Device* device, UINT objectCount, UINT timeCount, UINT particleCount);
	FrameResource(const FrameResource& rhs) = delete;
	FrameResource& operator=(const FrameResource& rhs) = delete;
	~FrameResource();

	// GPU がコマンドの処理を完了するまでアロケータをリセットできないため、各フレームには独自のアロケータが必要です
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandListAllocator;

	// GPU がそれを参照するコマンドの処理を完了するまで、cbuffer を更新することはできません
	// したがって、各フレームには独自の cbuffer が必要です
	std::unique_ptr<UploadBuffer<ObjectConstants>> ObjectCB = nullptr;
	std::unique_ptr<UploadBuffer<TimeConstants>> TimeCB = nullptr;
	std::unique_ptr<UploadBuffer<ParticleConstants>> ParticleCB = nullptr;

	// このフェンスポイントまでのコマンドをマークするフェンス値
	// これにより、これらのフレーム リソースが GPU によってまだ使用されているかどうかを確認できます
	UINT64 Fence = 0;
};