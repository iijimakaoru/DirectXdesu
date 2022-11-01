#pragma once
#include <DirectXMath.h>
#include "KDirectXCommon.h"
#include "Vector3.h"
#include "Vector2.h"
#include <vector>
#include "PipelineSet.h"
#include "ConstBuffer.h"
#include "KShader.h"
#include <memory>
#include <d3dx12.h>
#include "KSpriteCommon.h"

const int spriteSRVCount = 512;

struct VertexPosUV
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

struct SpriteInfo
{
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//	���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// Z������̉�]�p
	float rotation = 0.0f;
	// ���W
	Vector3 position = { 50,50,0 };
	// ���[���h�s��
	XMMATRIX matWorld;
	// �F
	XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
	// �e�N�X�`���ԍ�
	UINT texNum = 0;
	// �T�C�Y
	Vector2 size = { 100,100 };
	// �A���J�[�|�C���g
	Vector2 anchorpoint = { 0.5f,0.5f };
	// ���E���]
	bool isFlipX = false;
	// �㉺���]
	bool isFlipY = false;
	// �e�N�X�`������
	Vector2 texLeftTop = { 0,0 };
	// �e�N�X�`���؂�o���T�C�Y
	Vector2 texSize = { 100,100 };
	// �؂�o���t���O
	bool isKiridasi = false;
	// ��\��
	bool isInvisible = false;
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

class Sprite
{
public:
	void Init(SpriteCommon* spriteCommon);
	void Draw();
	void SpriteTransferVertexBuffer(const SpriteInfo& sprite, const SpriteCommond& spriteCommon);
	PipelineSet SpriteCreateGraphicsPipeline(ID3D12Device* dev);
	SpriteInfo SpriteCreate(ID3D12Device* dev, int window_width, int window_height, UINT texNumber, 
		const SpriteCommond& spriteCommon, Vector2 anchorpoint = { 0.5f,0.5f }, 
		bool isFlipX = false, bool isFlipY = false);
	void SpriteCommonBeginDraw(ID3D12GraphicsCommandList* cmdList, const SpriteCommond& spriteCommon);
	void SpriteDraw(const SpriteInfo& sprite, ID3D12GraphicsCommandList* cmdList,
		const SpriteCommond& spriteCommon, ID3D12Device* dev);
	SpriteCommond SpriteCommonCreate(ID3D12Device* dev, int window_width, int window_height);
	void SpriteUpdate(SpriteInfo& sprite, const SpriteCommond& spriteCommon);
	HRESULT SpriteCommonLoadTexture(SpriteCommond& spriteCommon,
		UINT texnumber, const wchar_t* filename, ID3D12Device* dev);

private:
	SpriteCommon* spriteCommon_ = nullptr;
	// 
	D3D12_VERTEX_BUFFER_VIEW vbView{};
};

