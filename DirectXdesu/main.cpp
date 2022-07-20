#include <Windows.h>
#include <stdio.h>
#include<vector>
#include<string>
#include "Window.h"
#include "DirectInit.h"
#include "Input.h"
#include "Enum.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>
#ifdef DEBUG
#include <iostream>
#endif
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.Lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace DirectX;

struct ConstBufferDataMaterial {
	XMFLOAT4 color;
};

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef DEBUG
	// �f�o�b�O���C���[���I����
	ID3D12Ddebug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif

#pragma region �E�B���h�E
	Window win;
#pragma endregion

#pragma region DirectX������
	DirectInit dx(win);
#pragma endregion

#pragma region DirectInput�̏�����
	Input input(dx.result, win.window, win.handle);
#pragma endregion

#pragma region �`�揉����

	// ���_�f�[�^
	XMFLOAT3 vertices[] = {
		{-0.5f,-0.5f,0.0f},// ����
		{-0.5f,+0.5f,0.0f},// ����
		{+0.5f,-0.5f,0.0f},// �E��
	};

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp[2]{};
	heapProp[0].Type = heapProp[1].Type = D3D12_HEAP_TYPE_UPLOAD;
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc[2]{};
	resDesc[0].Dimension = resDesc[1].Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc[0].Width = sizeVB;
	resDesc[0].Height = resDesc[1].Height = 1;
	resDesc[0].DepthOrArraySize = resDesc[1].DepthOrArraySize = 1;
	resDesc[0].MipLevels = resDesc[1].MipLevels = 1;
	resDesc[0].SampleDesc.Count = resDesc[1].SampleDesc.Count = 1;
	resDesc[0].Layout = resDesc[1].Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// ���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff = nullptr;
	dx.result = dx.dev->CreateCommittedResource(
		&heapProp[0],
		D3D12_HEAP_FLAG_NONE,
		&resDesc[0],
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(dx.result));

	// GPU��̃o�b�t�@�ɑΉ��������z���������擾
	XMFLOAT3* vertMap = nullptr;
	dx.result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(dx.result));

	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_����̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(XMFLOAT3);

	ID3D10Blob* vsBlob = nullptr; // ���_�V�F�[�_�[�I�u�W�F�N�g
	ID3D10Blob* psBlob = nullptr; // �s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ID3D10Blob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	dx.result = D3DCompileFromFile(
		L"BasicVS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob);

	// �G���[���ł���
	if (FAILED(dx.result))
	{
		// erroeBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	dx.result = D3DCompileFromFile(
		L"BasicPS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);

	// �G���[���ł���
	if (FAILED(dx.result))
	{
		// erroeBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc2{};

	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = pipelineDesc2.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = pipelineDesc2.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = pipelineDesc2.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = pipelineDesc2.PS.BytecodeLength = psBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = pipelineDesc2.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = pipelineDesc2.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	pipelineDesc.RasterizerState.DepthClipEnable = pipelineDesc2.RasterizerState.DepthClipEnable = true;

	// �u�����h�X�e�[�g
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = pipelineDesc2.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = pipelineDesc2.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = pipelineDesc2.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = pipelineDesc2.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = pipelineDesc2.NumRenderTargets = 1;
	pipelineDesc.RTVFormats[0] = pipelineDesc2.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	pipelineDesc.SampleDesc.Count = pipelineDesc2.SampleDesc.Count = 1;

	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	pipelineDesc2.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC cbResouceDesc{};
	cbResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResouceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	cbResouceDesc.Height = 1;
	cbResouceDesc.DepthOrArraySize = 1;
	cbResouceDesc.MipLevels = 1;
	cbResouceDesc.SampleDesc.Count = 1;
	cbResouceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ID3D12Resource* constBufferMaterial = nullptr;
	dx.result = dx.dev->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResouceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBufferMaterial));
	assert(SUCCEEDED(dx.result));

	ConstBufferDataMaterial* constMapMaterial = nullptr;
	dx.result = constBufferMaterial->Map(0, nullptr, (void**)&constBufferMaterial);
	assert(SUCCEEDED(dx.result));

	constMapMaterial->color = XMFLOAT4(1, 0, 0, 0.5f);

	D3D12_ROOT_PARAMETER rootParam = {};
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam.Descriptor.ShaderRegister = 0;
	rootParam.Descriptor.RegisterSpace = 0;
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// ���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;
	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParam;
	rootSignatureDesc.NumParameters = 1;
	// ���[�g�V�O�l�`���̃V���A���C�Y
	ID3DBlob* rootSigBlob = nullptr;
	dx.result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(dx.result));
	dx.result = dx.dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(dx.result));
	rootSigBlob->Release();

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature;
	pipelineDesc2.pRootSignature = rootSignature;

	// �p�C�v���C���X�e�[�g
	ID3D12PipelineState* pipelineState = { nullptr };
	ID3D12PipelineState* pipelineState2 = { nullptr };
	dx.result = dx.dev->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(dx.result));

	dx.result = dx.dev->CreateGraphicsPipelineState(&pipelineDesc2, IID_PPV_ARGS(&pipelineState2));
	assert(SUCCEEDED(dx.result));

	int isMode = Triangle;
	bool isChange = false;
#pragma endregion

	// �E�B���h�E�\��
	// �Q�[�����[�v
	while (true) {
		win.Update();
		input.Update(dx.result);
#pragma region �E�B���h�E���b�Z�[�W
		if (win.breakFlag || input.IsPush(DIK_ESCAPE)) {
			break;
		}
#pragma endregion

		// �X�V
#pragma region �L�[�{�[�h
		if (input.IsPush(DIK_SPACE)) {
			dx.bRed = 1.0f;
			dx.bGreen = 0.7f;
			dx.bBule = 1.0f;
		}
		else {
			dx.bRed = 0.1f;
			dx.bGreen = 0.25f;
			dx.bBule = 0.5f;
		}

		if (input.IsTriger(DIK_1)) {
			if (isMode == Triangle)
			{
				isMode = Box;
			}
			else {
				isMode = Triangle;
			}
		}

		if (input.IsTriger(DIK_2)) {
			if (!isChange) {
				isChange = true;
			}
			else {
				isChange = false;
			}
		}
#pragma endregion

		// �`��

#pragma region ���\�[�X�o���A
		// �o�b�N�o�b�t�@�̔ԍ����擾
		UINT bbIndex = dx.swapChain->GetCurrentBackBufferIndex();

		// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = dx.backBuffers[bbIndex];
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		dx.cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region �`���
		// 2. �`���̕ύX
		// �����_�[�^�[�Q�[�g�r���[�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx.rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * dx.dev->GetDescriptorHandleIncrementSize(dx.rtvHeapDesc.Type);
		dx.cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
#pragma endregion

#pragma region ��ʃN���A
		// 3. ��ʃN���A
		FLOAT clearColor[] = { dx.bRed,dx.bGreen,dx.bBule,0.0f };
		dx.cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
#pragma endregion

#pragma region �`��
		// �`��R�}���h��������
		// �r���[�|�[�g�ݒ�R�}���h
		D3D12_VIEWPORT viewport[8]{};
		viewport[0].Width = win.window_width;   // ����
		viewport[0].Height = win.window_height; // �c��
		viewport[0].TopLeftX = 0;                 // ����x
		viewport[0].TopLeftY = 0;				   // ����y
		viewport[0].MinDepth = 0.0f;			   // �ŏ��[�x
		viewport[0].MaxDepth = 1.0f;			   // �ő�[�x

		// �r���[�|�[�g�ݒ�R�}���h���R�}���h���X�g�ɐς�
		for (int i = 0; i < 8; i++) {
			dx.cmdList->RSSetViewports(1, &viewport[i]);

			// �V�U�[��`
			D3D12_RECT scissorRect{};
			scissorRect.left = 0;									// �؂蔲�����W��
			scissorRect.right = scissorRect.left + win.window_width;	// �؂蔲�����W�E
			scissorRect.top = 0;									// �؂蔲�����W��
			scissorRect.bottom = scissorRect.top + win.window_height;	// �؂蔲�����W��

			// �V�U�[��`�ݒ�R�}���h���R�}���h���X�g�ɐς�
			dx.cmdList->RSSetScissorRects(1, &scissorRect);

			// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
			dx.cmdList->SetPipelineState(pipelineState);

			dx.cmdList->SetGraphicsRootSignature(rootSignature);

			// �v���~�e�B�u�`��̐ݒ�R�}���h
			dx.cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
			dx.cmdList->IASetVertexBuffers(0, 1, &vbView);

			dx.cmdList->SetGraphicsRootConstantBufferView(0, constBufferMaterial->GetGPUVirtualAddress());

			// �`��R�}���h
			dx.cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);
		}
		// �`��R�}���h�����܂�
#pragma endregion

#pragma region ���\�[�X�o���A��߂�
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		dx.cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region �R�}���h�̃t���b�V��
		// ���߂̃N���[�Y
		dx.result = dx.cmdList->Close();
		assert(SUCCEEDED(dx.result));

		// �R�}���h���X�g�̎��s
		ID3D12CommandList* cmdLists[] = { dx.cmdList };
		dx.cmdQueue->ExecuteCommandLists(1, cmdLists);

		// ��ʂɕ\������o�b�t�@���t���b�v(���\�̓���ւ�)
		dx.result = dx.swapChain->Present(1, 0);
		assert(SUCCEEDED(dx.result));
#pragma endregion

#pragma region �R�}���h�����҂�
		// �R�}���h�̊�����҂�
		dx.cmdQueue->Signal(dx.fence, ++dx.fenceVal);
		if (dx.fence->GetCompletedValue() != dx.fenceVal)
		{
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			dx.fence->SetEventOnCompletion(dx.fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}

		// �L���[���N���A
		dx.result = dx.cmdAllocater->Reset();
		assert(SUCCEEDED(dx.result));
		// �ĂуR�}���h�𒙂߂鏀��
		dx.result = dx.cmdList->Reset(dx.cmdAllocater, nullptr);
		assert(SUCCEEDED(dx.result));
#pragma endregion

	}

	return 0;
}