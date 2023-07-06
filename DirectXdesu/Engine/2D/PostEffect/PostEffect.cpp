#include "PostEffect.h"
#include "KInput.h"

// �ÓI�����o�ϐ��̎���
KMyMath::Matrix4 PostEffect::matPro;
KGPlin* PostEffect::pipeline = nullptr;
ComPtr<ID3D12Device> PostEffect::device;
ComPtr<ID3D12GraphicsCommandList> PostEffect::cmdList;
KWinApp* PostEffect::window = nullptr;
const float PostEffect::clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.0f };

void PostEffect::StaticInit()
{
	device = KDirectXCommon::GetInstance()->GetDev();

	window = KWinApp::GetInstance();

	cmdList = KDirectXCommon::GetInstance()->GetCmdlist();

	float width = static_cast<float>(window->GetWindowSizeW());
	float height = static_cast<float>(window->GetWindowSizeH());

	matPro = MyMathUtility::MakeOrthogonalL(0.0f, width, height, 0.0f, 0.0f, 1.0f);
}

void PostEffect::Init()
{
	// �萔�o�b�t�@�}�e���A��
	CreateCBMaterial();

	// ���_
	CreateVertex();

	// �萔�o�b�t�@�g�����X�t�H�[��
	CreateCBTransform();

	// �e�N�X�`��
	CreateTextureBuff();

	// �[�x�o�b�t�@
	CreateDepthBuff();

	*constMapTransform = MyMathUtility::MakeIdentity();
}

void PostEffect::DrawCommand()
{
	// �f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// SRV�q�[�v�̐擪�n���h�����擾
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = descHeapSRV->GetGPUDescriptorHandleForHeapStart();
	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h(�}�e���A��)
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h(�g�����X�t�H�[��)
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
	cmdList->IASetVertexBuffers(0, 1, &vbView);
}

void PostEffect::PreDrawScene()
{
	for (size_t i = 0; i < 2; i++)
	{
		// ���\�[�X�o���A�ύX
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
		cmdList->ResourceBarrier(1, &barrier);
	}

	// �����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h���擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[2];
	for (size_t i = 0; i < 2; i++)
	{
		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV->GetCPUDescriptorHandleForHeapStart(), static_cast<INT>(i),
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}

	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h���擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();

	// �����_�[�^�[�Q�b�g���Z�b�g
	cmdList->OMSetRenderTargets(2, rtvHs, false, &dsvH);

	// �r���[�|�[�g�ݒ�
	CD3DX12_VIEWPORT viewPorts[2];

	// �V�U�����O��`�ݒ�
	CD3DX12_RECT rects[2];

	for (size_t i = 0; i < 2; i++)
	{
		viewPorts[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<FLOAT>(window->GetWindowSizeW()), static_cast<FLOAT>(window->GetWindowSizeH()));
		rects[i] = CD3DX12_RECT(0, 0, static_cast<LONG>(window->GetWindowSizeW()), static_cast<LONG>(window->GetWindowSizeH()));
	}
	cmdList->RSSetViewports(2, viewPorts);
	cmdList->RSSetScissorRects(2, rects);

	// �w�ʃN���A
	for (size_t i = 0; i < 2; i++)
	{
		cmdList->ClearRenderTargetView(rtvHs[i], clearColor, 0, nullptr);
	}

	// �[�x�o�b�t�@�̃N���A
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene()
{
	for (size_t i = 0; i < 2; i++)
	{
		// ���\�[�X�o���A��ύX
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		cmdList->ResourceBarrier(1, &barrier);
	}
}

void PostEffect::Draw()
{
	// �p�C�v���C���Z�b�g
	pipeline->Setting();
	pipeline->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); // �O�p�`���X�g

	if (KInput::GetInstance()->IsTrigger(DIK_0))
	{
		// �f�X�N���v�^�q�[�v��SRV�쐬
		static size_t tex = 0;

		tex = (tex + 1) % 2;

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		device->CreateShaderResourceView(texBuff[tex].Get(),
			&srvDesc,
			descHeapSRV->GetCPUDescriptorHandleForHeapStart());
	}

	// �`��̏���
	DrawCommand();

	// �`��R�}���h
	cmdList->DrawInstanced(_countof(vertices_), 1, 0, 0);
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

void PostEffect::CreateVertex()
{
	// �q�[�v�ݒ�
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̐�
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * 4);

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

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

	// ���_�f�[�^
	Vertex vertices[4] =
	{
		{{-1.0f, 1.0f,0.0f},{0.0f,0.0f}}, // ����
		{{-1.0f,-1.0f,0.0f},{0.0f,1.0f}}, // ����
		{{ 1.0f, 1.0f,0.0f},{1.0f,0.0f}}, // �E��
		{{ 1.0f,-1.0f,0.0f},{1.0f,1.0f}}, // �E��
	};

	for (size_t i = 0; i < 4; i++)
	{
		vertices_[i] = vertices[i];
	}

	// ���_�}�b�v
	Vertex* vertMap = nullptr;

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		memcpy(vertMap, vertices_, sizeof(vertices_));
		vertBuff->Unmap(0, nullptr);
	}
	assert(SUCCEEDED(result));

	// ���_�o�b�t�@�r���[�̍쐬
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_����̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(Vertex);
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

void PostEffect::CreateTextureBuff()
{
	// �e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		window->GetWindowSizeW(),
		(UINT)window->GetWindowSizeH(),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	CD3DX12_CLEAR_VALUE clear = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);

	for (size_t i = 0; i < 2; i++)
	{
		// �e�N�X�`���o�b�t�@�̐���
		result = device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clear,
			IID_PPV_ARGS(&texBuff[i])
		);
		assert(SUCCEEDED(result));

		{// �e�N�X�`���ԃN���A
			// ��f�� �E�B���h�E�� �~ �E�B���h�E�c = ���s�N�Z����
			const UINT pixcelCount = window->GetWindowSizeW() * window->GetWindowSizeH();

			// ��f1�s���̃f�[�^�T�C�Y
			const UINT rowPitch = sizeof(UINT) * window->GetWindowSizeW();

			// �摜�S�̂̃f�[�^�T�C�Y
			const UINT depthPitch = rowPitch * window->GetWindowSizeH();

			// �摜�C���[�W
			UINT* img = new UINT[pixcelCount];
			for (size_t j = 0; j < pixcelCount; j++)
			{
				img[j] = 0xff0000ff;
			}

			// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
			result = texBuff[i]->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
			assert(SUCCEEDED(result));
			delete[] img;
		}
	}

	// SRV�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;

	// SRV�p�f�X�N���v�^�q�[�v����
	result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	// SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	// �f�X�N���v�^�q�[�v��SRV�쐬
	device->CreateShaderResourceView(texBuff[0].Get(), // �r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::CreateDepthBuff()
{
	// RTV�p�f�X�N���v�^�q�[�v�쐬
	CreateRTVDescHeap();

	// DSV�p�f�X�N���v�^�q�[�v����
	CreateDSVDescHeap();
}

void PostEffect::CreateDSVDescHeap()
{
	// DSV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC dsvDescHeapDesc{};
	dsvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvDescHeapDesc.NumDescriptors = 1;

	// DSV�p�f�X�N���v�^�q�[�v����
	result = device->CreateDescriptorHeap(&dsvDescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	// �f�X�N���v�^�q�[�v��DSV����
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::CreateRTVDescHeap()
{
	// RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 2;

	// RTV�p�f�X�N���v�^�q�[�v����
	result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	// �����_�[�^�[�Q�b�g�r���[�ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};

	// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	for (size_t i = 0; i < 2; i++)
	{
		// �f�X�N���v�^�q�[�v��RTV�쐬
		device->CreateRenderTargetView(texBuff[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(),static_cast<INT>(i),
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
	}

	// �[�x�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			static_cast<UINT>(window->GetWindowSizeW()),
			static_cast<UINT>(window->GetWindowSizeH()),
			1, 0, 1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);

	CD3DX12_HEAP_PROPERTIES heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	CD3DX12_CLEAR_VALUE clear = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	// �[�x�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heap,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clear,
		IID_PPV_ARGS(&depthBuff)
	);
	assert(SUCCEEDED(result));
}
