#include "PostEffect.h"
#include "KWinApp.h"

KMyMath::Matrix4 PostEffect::matPro;
KGPlin* PostEffect::pipeline = nullptr;
ComPtr<ID3D12Device> PostEffect::device;
ComPtr<ID3D12GraphicsCommandList> PostEffect::cmdList;

void PostEffect::StaticInit()
{
	device = KDirectXCommon::GetInstance()->GetDev();

	cmdList = KDirectXCommon::GetInstance()->GetCmdlist();

	float width = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeW());
	float height = static_cast<float>(KWinApp::GetInstance()->GetWindowSizeH());

	matPro = MyMathUtility::MakeOrthogonalL(0.0f, width, height, 0.0f, 0.0f, 1.0f);
}

void PostEffect::Init()
{
	// �萔�o�b�t�@�}�e���A��
	CreateCBMaterial();

	// ���_�A�C���f�b�N�X
	CreateVertexIndex();

	// �萔�o�b�t�@�g�����X�t�H�[��
	CreateCBTransform();

	*constMapTransform = MyMathUtility::MakeIdentity();
}

void PostEffect::Update(KMyMath::Vector2 pos, KMyMath::Vector2 scale, float rot, KMyMath::Vector4 color)
{
	// ���[���h�ϊ�
	KMyMath::Matrix4 matWorld, matTrans, matRot;
	// �ړ��s��
	matTrans = MyMathUtility::MakeTranslation({ pos.x,pos.y,0.0f });
	// ��]�s��
	matRot = MyMathUtility::MakeRotation({ 0.0f,0.0f,XMConvertToRadians(rot) });

	// �S�č���
	matWorld = matRot * matTrans;

	// �萔�o�b�t�@�֓]��
	*constMapTransform = matWorld * matPro;

	// �F�̑��
	*constMapMaterial = color;
}

void PostEffect::DrawCommand(KTexture* texture)
{
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
}

void PostEffect::Draw(KTexture* texture, KMyMath::Vector2 pos, KMyMath::Vector2 setSize_, float rot, KMyMath::Vector4 color,
	bool isFlipX_, bool isFlipY_, KMyMath::Vector2 anchorPoint_)
{
	// ��\������
	if (isInvisible)
	{
		return;
	}

	if (isFlipX_)
	{
		flipX = -1;
	}
	else
	{
		flipX = 1;
	}

	if (isFlipY_)
	{
		flipY = -1;
	}
	else
	{
		flipY = 1;
	}

	// �A���J�[�|�C���g
	float left = ((0.0f - anchorPoint_.x) * setSize_.x) * flipX;
	float right = ((1.0f - anchorPoint_.x) * setSize_.x) * flipX;
	float top = ((0.0f - anchorPoint_.y) * setSize_.y) * flipY;
	float bottom = ((1.0f - anchorPoint_.y) * setSize_.y) * flipY;

	// ���_�f�[�^
	Vertex vertices[] =
	{
		{{ left,   top,0.0f},{0.0f,0.0f}}, // ����
		{{ left,bottom,0.0f},{0.0f,1.0f}}, // ����
		{{right,   top,0.0f},{1.0f,0.0f}}, // �E��
		{{right,bottom,0.0f},{1.0f,1.0f}}, // �E��
	};

	// �C���f�b�N�X�f�[�^
	uint16_t indices[] =
	{
		1,0,3, // �O�p�`1��
		2,3,0, // �O�p�`2��
	};

	// �S���_�ɑ΂���
	std::copy(std::begin(vertices), std::end(vertices), vertMap);

	// �S�C���f�b�N�X�ɑ΂���
	std::copy(std::begin(indices), std::end(indices), indexMap);

	Update(pos, setSize_, rot, color);

	// �p�C�v���C���Z�b�g
	pipeline->Setting();
	pipeline->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �O�p�`���X�g

	// �`��̏���
	DrawCommand(texture);

	// �`��R�}���h
	cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}

void PostEffect::SetPipeline(KGPlin* pipeline_)
{
	pipeline = pipeline_;
}

void PostEffect::CreateCBMaterial()
{
	// �萔�o�b�t�@�����p
	D3D12_HEAP_PROPERTIES cbHeapProp{}; // �q�[�v�̐ݒ�
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(KMyMath::Vector4) + 0xff) & ~0xff; // 256�o�C�g�A���C�����g
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
		IID_PPV_ARGS(constBuffMaterial.ReleaseAndGetAddressOf())
	);
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̃}�b�s���O
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // �}�b�s���O
	assert(SUCCEEDED(result));

	// �l�̑��
	*constMapMaterial = KMyMath::Vector4(1.0f, 0.5f, 0.5f, 1.0f);
}

void PostEffect::CreateVertexIndex()
{
	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̐�
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * 4);

	// ���_�o�b�t�@�̐ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]��

	// ���\�[�X�ݒ�
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
		IID_PPV_ARGS(vertBuff.ReleaseAndGetAddressOf())
	);
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_����̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(Vertex);

	// �C���f�b�N�X�f�[�^�̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * 6);

	// ���_�o�b�t�@�̐ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]��

	// ���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;
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
		IID_PPV_ARGS(indexBuff.ReleaseAndGetAddressOf())
	);
	assert(SUCCEEDED(result));

	// �C���f�b�N�X�o�b�t�@�̃}�b�s���O
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));

	// �}�b�s���O����
	indexBuff->Unmap(0, nullptr);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	// ���_�o�b�t�@�̃T�C�Y
	ibView.SizeInBytes = sizeVB;
	assert(SUCCEEDED(result));
}

void PostEffect::CreateCBTransform()
{
	// �萔�o�b�t�@�����p
	D3D12_HEAP_PROPERTIES cbHeapProp{}; // �q�[�v�̐ݒ�
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResDesc{};
	cbResDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResDesc.Width = (sizeof(KMyMath::Matrix4) + 0xff) & ~0xff; // ���_�f�[�^�S�̂̃T�C�Y
	cbResDesc.Height = 1;
	cbResDesc.DepthOrArraySize = 1;
	cbResDesc.MipLevels = 1;
	cbResDesc.SampleDesc.Count = 1;
	cbResDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&cbHeapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(constBuffTransform.ReleaseAndGetAddressOf())
	);
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform); // �}�b�s���O
	assert(SUCCEEDED(result));
}
