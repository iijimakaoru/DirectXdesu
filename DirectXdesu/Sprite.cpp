#include "Sprite.h"
#include "KShader.h"

void Sprite::Init(SpriteCommon* spriteCommon)
{
	assert(spriteCommon);
	spriteCommon_ = spriteCommon;


}

void Sprite::SpriteTransferVertexBuffer()
{
	HRESULT result = S_FALSE;

	VertexPosUV vertices[] = {
		{{},{0.0f,1.0f}}, // ����
		{{},{0.0f,0.0f}}, // ����
		{{},{1.0f,1.0f}}, // �E��
		{{},{1.0f,0.0f}}, // �E��
	};

	enum { LB, LT, RB, RT };

	float left = (0.0f - sprite_.anchorpoint.x) * sprite_.size.x;
	float right = (1.0f - sprite_.anchorpoint.x) * sprite_.size.x;
	float top = (0.0f - sprite_.anchorpoint.y) * sprite_.size.y;
	float bottom = (1.0f - sprite_.anchorpoint.y) * sprite_.size.y;

	if (sprite_.isFlipX)
	{
		left = -left;
		right = -right;
	}

	if (sprite_.isFlipY)
	{
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,bottom,0.0f };
	vertices[LT].pos = { left,	  top,0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,	  top,0.0f };

	if (sprite_.isKiridasi)
	{
		if (spriteCommon_->GetSpriteCommond().texBuff[sprite_.texNum])
		{
			// 
			D3D12_RESOURCE_DESC resDesc = spriteCommon_->GetSpriteCommond().texBuff[sprite_.texNum]->GetDesc();

			float tex_left = sprite_.texLeftTop.x / resDesc.Width;
			float tex_right = (sprite_.texLeftTop.x + sprite_.texSize.x) / resDesc.Width;
			float tex_top = sprite_.texLeftTop.y / resDesc.Height;
			float tex_bottom = (sprite_.texLeftTop.y + sprite_.texSize.y) / resDesc.Height;

			vertices[LB].uv = { tex_left,tex_bottom };
			vertices[LT].uv = { tex_left,tex_top };
			vertices[RB].uv = { tex_right,tex_bottom };
			vertices[RT].uv = { tex_right,tex_top };
		}
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUV* vertMap = nullptr;
	result = sprite_.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite_.vertBuff->Unmap(0, nullptr);
}

SpriteInfo Sprite::SpriteCreate(UINT texNumber,  Vector2 anchorpoint, bool isFlipX, bool isFlipY)
{
	HRESULT result = S_FALSE;
	// �V�����X�v���C�g�����
	SpriteInfo sprite{};
	// ���_�f�[�^
	VertexPosUV vertices[] =
	{
		// x	  y		 z		  u	   v
		{{  0.0f,100.0f,  0.0f},{0.0f,1.0f}}, // ���� 
		{{  0.0f,  0.0f,  0.0f},{0.0f,0.0f}}, // ����
		{{100.0f,100.0f,  0.0f},{1.0f,1.0f}}, // �E��
		{{100.0f,  0.0f,  0.0f},{1.0f,0.0f}}, // �E��
	};
	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = static_cast<UINT>(sizeof(vertices));
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �e�N�X�`���ԍ��R�s�[
	sprite.texNum = texNumber;

	// ���_�o�b�t�@����
	result = spriteCommon_->GetDev()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&sprite.vertBuff));

	// �w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	if (spriteCommon_->GetSpriteCommond().texBuff[sprite.texNum])
	{
		// 
		D3D12_RESOURCE_DESC resDesc = spriteCommon_->GetSpriteCommond().texBuff[sprite.texNum]->GetDesc();
		// 
		sprite.size = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// ���]�t���O�R�s�[
	sprite.isFlipX = isFlipX;
	sprite.isFlipY = isFlipY;

	// �A���J�[�|�C���g�R�s�[
	sprite.anchorpoint = anchorpoint;

	// ���_�o�b�t�@�f�[�^�]��
	SpriteTransferVertexBuffer();

	// ���_�o�b�t�@�r���[�̍쐬
	sprite.vbView.BufferLocation = sprite.vertBuff->GetGPUVirtualAddress();
	sprite.vbView.SizeInBytes = sizeof(vertices);
	sprite.vbView.StrideInBytes = sizeof(vertices[0]);
	// �萔�o�b�t�@�̐���
	result = spriteCommon_->GetDev()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&sprite.constBuff)
	);
	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = sprite.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1); // �F�w��
	// ���s���e�s��
	constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, KWinApp::window_width, KWinApp::window_height, 0.0f, 0.0f, 1.0f);
	sprite.constBuff->Unmap(0, nullptr);

	return sprite;
}

void Sprite::SpriteDraw()
{
	// ��\���t���O
	if (sprite_.isInvisible)
	{
		return;
	}
	// ���_�o�b�t�@���Z�b�g
	spriteCommon_->GetCmdList()->IASetVertexBuffers(0, 1, &sprite_.vbView);
	// �萔�o�b�t�@���Z�b�g
	spriteCommon_->GetCmdList()->SetGraphicsRootConstantBufferView(0, sprite_.constBuff->GetGPUVirtualAddress());
	// �V�F�[�_�[���\�[�X�r���[���Z�b�g
	spriteCommon_->GetCmdList()->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			spriteCommon_->GetSpriteCommond().descHeap->GetGPUDescriptorHandleForHeapStart(),
			sprite_.texNum,
			spriteCommon_->GetDev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	// �|���S���̕`��
	spriteCommon_->GetCmdList()->DrawInstanced(4, 1, 0, 0);
}

void Sprite::SpriteUpdate()
{
	// ���[���h�s��
	sprite_.matWorld = XMMatrixIdentity();
	// Z����]
	sprite_.matWorld *= XMMatrixRotationZ(XMConvertToRadians(sprite_.rotation));
	// ���s�ړ�
	sprite_.matWorld *= XMMatrixTranslation(sprite_.position.x, sprite_.position.y, sprite_.position.z);
	// �萔�o�b�t�@�̓]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = sprite_.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = sprite_.matWorld * spriteCommon_->GetSpriteCommond().matProjection;
	constMap->color = sprite_.color;
	sprite_.constBuff->Unmap(0, nullptr);
}
