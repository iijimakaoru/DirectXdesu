#include "Sprite.h"
#include "KDirectXCommon.h"
#include "KWinApp.h"

void SpriteCommon::Init()
{
	device = KDirectXCommon::GetInstance()->GetDev();
}

void SpriteCommon::Update()
{
	
}

void SpriteCommon::Draw()
{
	// �p�C�v���C���Z�b�g
	pipeline->Setting();
	pipeline->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �O�p�`���X�g
}

SpriteCommon* SpriteCommon::GetInstance()
{
	static SpriteCommon instance;
	return &instance;
}

void SpriteCommon::SetPipeline(KGPlin* pipeline_)
{
	pipeline = pipeline_;
}

void Sprite::CreateCBMaterial()
{
	// �萔�o�b�t�@�����p
	D3D12_HEAP_PROPERTIES cbHeapProp{}; // �q�[�v�̐ݒ�
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff; // 256�o�C�g�A���C�����g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&cbHeapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial)
	);
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̃}�b�s���O
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // �}�b�s���O
	assert(SUCCEEDED(result));

	// �l�̑��
	constMapMaterial->color = DirectX::XMFLOAT4(0.5f, 0.5f, 0.0f, 0.5f);
}

void Sprite::CreateCBTransform()
{
	// �萔�o�b�t�@�����p
	D3D12_HEAP_PROPERTIES cbHeapProp{}; // �q�[�v�̐ݒ�
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(KConstBufferDataTransform) + 0xff) & ~0xff; // 256�o�C�g�A���C�����g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&cbHeapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform); // �}�b�s���O
	assert(SUCCEEDED(result));

	// �P�ʍs�����
	constMapTransform->mat = XMMatrixIdentity();
	constMapTransform->mat.r[0].m128_f32[0] = 2.0f / static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	constMapTransform->mat.r[1].m128_f32[1] = -2.0f / static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());
	constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	constMapTransform->mat.r[3].m128_f32[1] = 1.0f;
}

void Sprite::Init()
{
	spriteCommon = SpriteCommon::GetInstance();

	device = KDirectXCommon::GetInstance()->GetDev();

	// ���_�f�[�^
	vertices =
	{
		{{  0.0f,100.0f,0.0f},{0.0f,1.0f}}, // ����
		{{  0.0f,  0.0f,0.0f},{0.0f,0.0f}}, // ����
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}}, // �E��
		{{100.0f,  0.0f,0.0f},{1.0f,0.0f}}, // �E��
	};

	// ���_�f�[�^�S�̂̃T�C�Y
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]��

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐ݒ�
	result = device->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	std::copy(std::begin(vertices), std::end(vertices), vertMap);
	// �q���������
	vertBuff->Unmap(0, nullptr);

	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_����̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);

	// �萔�o�b�t�@�}�e���A��
	CreateCBMaterial();

	// �萔�o�b�t�@�g�����X�t�H�[��
	CreateCBTransform();
}

void Sprite::Draw(KTexture* texture)
{
	spriteCommon->Draw();

	ID3D12GraphicsCommandList* cmdList = KDirectXCommon::GetInstance()->GetCmdlist();

	// �f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { texture->srvHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// SRV�q�[�v�̐擪�n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = texture->srvHeap->GetGPUDescriptorHandleForHeapStart();
	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h(�}�e���A��)
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h(�g�����X�t�H�[��)
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// �`��R�}���h
	cmdList->DrawInstanced(vertices.size(), 1, 0, 0);
}

void Sprite::Update()
{
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// �A���J�[�|�C���g
	float left =   (0.0f - anchorPoint.x) * size_.x;
	float right =  (1.0f - anchorPoint.x) * size_.x;
	float top =    (0.0f - anchorPoint.y) * size_.y;
	float bottom = (1.0f - anchorPoint.y) * size_.y;

	// ���E���]
	if (isFlipX)
	{
		left = -left;
		right = -right;
	}
	// �㉺���]
	if (isFlipY)
	{
		top = -top;
		bottom = -bottom;
	}

	// ���_�f�[�^
	vertices[LB].pos = {  left, bottom, 0.0f };
	vertices[LT].pos = {  left,	   top, 0.0f };
	vertices[RB].pos = { right, bottom, 0.0f };
	vertices[RT].pos = { right,	   top, 0.0f };

	// �S���_�ɑ΂���
	std::copy(std::begin(vertices), std::end(vertices), vertMap);
	// �q���������
	vertBuff->Unmap(0, nullptr);

	// ���[���h�ϊ�
	DirectX::XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();
	// ��]
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	// ���s�ړ�
	matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	// �萔�o�b�t�@�֓]��
	constMapTransform->mat.r[0].m128_f32[0] = 2.0f / static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	constMapTransform->mat.r[1].m128_f32[1] = -2.0f / static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());
	constMapTransform->mat.r[3].m128_f32[0] = -1.0f;
	constMapTransform->mat.r[3].m128_f32[1] = 1.0f;
	constMapTransform->mat *= matWorld;

	// �F�̑��
	constMapMaterial->color = color;
}
