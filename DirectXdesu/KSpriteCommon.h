#pragma once
#include "KDirectXCommon.h"
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>
#include "Vector3.h"
#include "Vector2.h"
#include "PipelineSet.h"

const int spriteSRVCount = 512;

struct VertexPosUV
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

struct SpriteCommond
{
	// �p�C�v���C���Z�b�g
	PipelineSet pipelineSet;
	// �ˉe�s��
	XMMATRIX matProjection{};
	// �e�N�X�`���p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// �e�N�X�`�����\�[�X(�e�N�X�`���o�b�t�@)�̔z��
	ComPtr<ID3D12Resource> texBuff[spriteSRVCount];
};

class SpriteCommon
{
public:
	void Init(KDirectXCommon* dxCommon);
	void BeginDraw();

	ID3D12Device* GetDev() const { return dxCommon_->GetDev().Get(); }
	ID3D12GraphicsCommandList* GetCmdList() { return dxCommon_->GetCmdlist().Get(); }
	SpriteCommond GetSpriteCommond() { return spriteCommond_; }

	PipelineSet SpriteCreateGraphicsPipeline();
	
	void SpriteCommonBeginDraw();
	
	SpriteCommond SpriteCommonCreate();
	
	HRESULT SpriteCommonLoadTexture(SpriteCommond& spriteCommon, UINT texnumber, const wchar_t* filename);

private:
	KDirectXCommon* dxCommon_ = nullptr;

	SpriteCommond spriteCommond_;
};

