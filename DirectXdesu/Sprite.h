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
#include "SpriteCommon.h"

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
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="dxCommon"></param>
	void Init(KDirectXCommon* dxCommon);
	/// <summary>
	/// ���_�o�b�t�@
	/// </summary>
	/// <param name="sprite"></param>
	/// <param name="spriteCommon"></param>
	void SpriteTransferVertexBuffer(const SpriteInfo& sprite, const SpriteCommon& spriteCommon);
	/// <summary>
	/// �X�v���C�g����
	/// </summary>
	/// <param name="texNumber"></param>
	/// <param name="spriteCommon"></param>
	/// <param name="anchorpoint"></param>
	/// <param name="isFlipX"></param>
	/// <param name="isFlipY"></param>
	/// <returns></returns>
	SpriteInfo SpriteCreate(UINT texNumber, const SpriteCommon& spriteCommon, Vector2 anchorpoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);
	/// <summary>
	/// �X�v���C�g��PreDraw
	/// </summary>
	/// <param name="spriteCommon"></param>
	void SpriteCommonBeginDraw(const SpriteCommon& spriteCommon);
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="sprite"></param>
	/// <param name="spriteCommon"></param>
	void SpriteDraw(const SpriteInfo& sprite, const SpriteCommon& spriteCommon);
	/// <summary>
	/// SpriteCommon����
	/// </summary>
	/// <returns></returns>
	SpriteCommon SpriteCommonCreate();
	/// <summary>
	/// �X�v���C�g�̍X�V
	/// </summary>
	/// <param name="sprite"></param>
	/// <param name="spriteCommon"></param>
	void SpriteUpdate(SpriteInfo& sprite, const SpriteCommon& spriteCommon);
	/// <summary>
	/// �e�N�X�`���̓ǂݍ���
	/// </summary>
	/// <param name="spriteCommon"></param>
	/// <param name="texnumber"></param>
	/// <param name="filename"></param>
	/// <returns></returns>
	HRESULT SpriteCommonLoadTexture(SpriteCommon& spriteCommon, UINT texnumber, const wchar_t* filename);
	/// <summary>
	/// �p�C�v���C���Z�b�^�[
	/// </summary>
	/// <param name="pipeline_"></param>
	void SetPipeline(KGPlin* pipeline_);

private:
	KDirectXCommon* dxCommon_ = nullptr;

	static KGPlin* pipeline;
};