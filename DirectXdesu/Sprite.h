#pragma once
#include <DirectXMath.h>
#include "KDirectXCommon.h"
#include <vector>
#include "ConstBuffer.h"
#include "KShader.h"
#include <memory>
#include <d3dx12.h>
#include "KSpriteCommon.h"

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

class Sprite
{
public:
	void Init(SpriteCommon* spriteCommon);

	void SpriteTransferVertexBuffer();
	SpriteInfo SpriteCreate(UINT texNumber,
		Vector2 anchorpoint = { 0.5f,0.5f },
		bool isFlipX = false, bool isFlipY = false);

	void SpriteDraw();
	void SpriteUpdate();

private:
	SpriteCommon* spriteCommon_ = nullptr;

	SpriteInfo sprite_;
};

