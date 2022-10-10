#pragma once
#include <DirectXMath.h>
#include "KDirectInit.h"
#include "Vector3.h"
#include "Vector2.h"
#include <vector>
#include "PipelineSet.h"
#include "ConstBuffer.h"
#include "KShader.h"
#include <memory>
#include <d3dx12.h>

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
	Vector3 position = { 0,0,0 };
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
	// ��\��
	bool isInvisible = false;
};

struct SpriteCommon
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
	void Init(ID3D12Device* dev, int window_width, int window_height);
	void SpriteTransferVertexBuffer(const SpriteInfo& sprite, const SpriteCommon& spriteCommon);
	PipelineSet SpriteCreateGraphicsPipeline(ID3D12Device* dev);
	SpriteInfo SpriteCreate(ID3D12Device* dev, int window_width, int window_height, UINT texNumber, 
		const SpriteCommon& spriteCommon, Vector2 anchorpoint = { 0.5f,0.5f }, 
		bool isFlipX = false, bool isFlipY = false);
	void SpriteCommonBeginDraw(ID3D12GraphicsCommandList* cmdList, const SpriteCommon& spriteCommon);
	void SpriteDraw(const SpriteInfo& sprite, ID3D12GraphicsCommandList* cmdList,
		const SpriteCommon& spriteCommon, ID3D12Device* dev);
	SpriteCommon SpriteCommonCreate(ID3D12Device* dev, int window_width, int window_height);
	void SpriteUpdate(SpriteInfo& sprite, const SpriteCommon& spriteCommon);
	HRESULT SpriteCommonLoadTexture(SpriteCommon& spriteCommon,
		UINT texnumber, const wchar_t* filename, ID3D12Device* dev);

private:
	
};

