#include <Windows.h>
#include <stdio.h>
#include<vector>
#include<string>
#include "Window.h"
#include "Dx12.h"
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

	// �ϐ�
#pragma region �E�B���h�E
	Window win;
#pragma endregion

#pragma region DirectX������
	Dx12* directX12 = new Dx12(win);

	//HRESULT result;
	//ID3D12Device* dev = nullptr;
	//IDXGIFactory6* dxgiFactory = nullptr;
	//IDXGISwapChain4* swapChain = nullptr;
	//ID3D12CommandAllocator* cmdAllocater = nullptr;
	//ID3D12GraphicsCommandList* cmdList = nullptr;
	//ID3D12CommandQueue* cmdQueue = nullptr;
	//ID3D12DescriptorHeap* rtvHeap = nullptr;
	//float bRed = 0.1f;
	//float bGreen = 0.25f;
	//float bBule = 0.5f;

	//// DXGI�t�@�N�g���[�̐���
	//result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	//assert(SUCCEEDED(result));

#pragma region �A�_�v�^
	//// �A�_�v�^�[�̗񋓗p
	//std::vector<IDXGIAdapter4*> adapters;
	//// �����ɓ���̃A�_�v�^�[�I�u�W�F�N�g������
	//IDXGIAdapter4* tmpAdapter = nullptr;

	//// �Ó��ȃA�_�v�^��I�ʂ���
	//for (size_t i = 0; i < adapters.size(); i++)
	//{
	//	DXGI_ADAPTER_DESC3 adapterDesc;
	//	// �A�_�v�^�[�̏����擾
	//	adapters[i]->GetDesc3(&adapterDesc);

	//	// �\�t�g�E�F�A�f�o�C�X�����
	//	if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
	//	{
	//		// �f�o�C�X���̗p���ă��[�v�𔲂���
	//		tmpAdapter = adapters[i];
	//		break;
	//	}
	//}

	//// �p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���
	//for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
	//	DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
	//	IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND; i++)
	//{
	//	// ���I�z��ɒǉ�
	//	adapters.push_back(tmpAdapter);
	//}
#pragma endregion

#pragma region �f�o�C�X
	//// �Ή����x���̔z��
	//D3D_FEATURE_LEVEL levels[] =
	//{
	//	D3D_FEATURE_LEVEL_12_1,
	//	D3D_FEATURE_LEVEL_12_0,
	//	D3D_FEATURE_LEVEL_11_1,
	//	D3D_FEATURE_LEVEL_11_0
	//};

	//D3D_FEATURE_LEVEL featureLevel;

	//for (size_t i = 0; i < _countof(levels); i++)
	//{
	//	result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&dev));
	//	if (result == S_OK)
	//	{
	//		// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
	//		featureLevel = levels[i];
	//		break;
	//	}
	//}
#pragma endregion

#pragma region �R�}���h���X�g
	//// �R�}���h�A���P�[�^�[����
	//result = dev->CreateCommandAllocator(
	//	D3D12_COMMAND_LIST_TYPE_DIRECT,
	//	IID_PPV_ARGS(&cmdAllocater));
	//assert(SUCCEEDED(result));

	//// �R�}���h���X�g����
	//result = dev->CreateCommandList(0,
	//	D3D12_COMMAND_LIST_TYPE_DIRECT,
	//	cmdAllocater, nullptr,
	//	IID_PPV_ARGS(&cmdList));
	//assert(SUCCEEDED(result));
#pragma endregion

#pragma region �R�}���h�L���[
	//// �R�}���h�L���[�̐ݒ�
	//D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	//// �R�}���h�L���[�𐶐�
	//result = dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
	//assert(SUCCEEDED(result));
#pragma endregion

#pragma region �X���b�v�`�F�[��
	//// �X���b�v�`�F�[���̐ݒ�
	//DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	//swapChainDesc.Width = 1280;
	//swapChainDesc.Height = 720;
	//swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//swapChainDesc.SampleDesc.Count = 1;
	//swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	//swapChainDesc.BufferCount = 2;
	//swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	//swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	//// �X���b�v�`�F�[���̐���
	//result = dxgiFactory->CreateSwapChainForHwnd(
	//	cmdQueue, win->handle, &swapChainDesc, nullptr, nullptr,
	//	(IDXGISwapChain1**)&swapChain);
	//assert(SUCCEEDED(result));
#pragma endregion

#pragma region �f�X�N���v�^�q�[�v
	//// �f�X�N���v�^�q�[�v�̐ݒ�
	//D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	//rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	//rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;
	//// �f�X�N���v�^�q�[�v�̐���
	//dev->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
#pragma endregion

#pragma region �o�b�N�o�b�t�@
	//// �o�b�N�o�b�t�@
	//std::vector<ID3D12Resource*> backBuffers;
	//backBuffers.resize(swapChainDesc.BufferCount);
#pragma endregion

#pragma region �����_�[�^�[�Q�[�g�r���[
	//// �X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	//for (size_t i = 0; i < backBuffers.size(); i++)
	//{
	//	// �X���b�v�`�F�[������o�b�t�@���擾
	//	swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
	//	// �f�X�N���v�^�q�[�v�̃n���h�����擾
	//	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	//	// �����\���ŃA�h���X�������
	//	rtvHandle.ptr += i * dev->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
	//	// �����_�[�^�[�Q�[�g�r���[�̐ݒ�
	//	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//	// �V�F�[�_�[�̌v�Z���ʂ�SKGB�ɕϊ����ď�������
	//	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	//	// �����_�[�^�[�Q�[�g�r���[�̐���
	//	dev->CreateRenderTargetView(backBuffers[i], &rtvDesc, rtvHandle);
	//}
#pragma endregion

#pragma region �t�F���X
	//// �t�F���X�̐���
	//ID3D12Fence* fence = nullptr;
	//UINT64 fenceVal = 0;

	//result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
#pragma endregion

#pragma region DirectInput�̏�����
	IDirectInput8* directInput = nullptr;
	result = DirectInput8Create(
		win->window.hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput,
		nullptr
	);
	assert(SUCCEEDED(result));
	// �L�[�{�[�h�f�o�C�X�̐���
	IDirectInputDevice8* keyboad = nullptr;
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboad, NULL);
	assert(SUCCEEDED(result));
	// ���̓f�[�^�`���̃Z�b�g
	result = keyboad->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	// �r�����䃌�x���̃Z�b�g
	result = keyboad->SetCooperativeLevel(
		win->handle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	assert(SUCCEEDED(result));
#pragma endregion
#pragma endregion

#pragma region �`�揉����

	// ���_�f�[�^
	XMFLOAT3 vertices[] = {
		{-0.5f,-0.5f,0.0f},// ����
		{-0.5f,+0.5f,0.0f},// ����
		{+0.5f,-0.5f,0.0f} // �E��
	};

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// ���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff = nullptr;
	result = dev->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z���������擾
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
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
	result = D3DCompileFromFile(
		L"BasicVS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob);

	// �G���[���ł���
	if (FAILED(result))
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
	result = D3DCompileFromFile(
		L"BasicPS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);

	// �G���[���ł���
	if (FAILED(result))
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

	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	pipelineDesc.RasterizerState.DepthClipEnable = true;

	// �u�����h�X�e�[�g
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	pipelineDesc.SampleDesc.Count = 1;

	// ���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;
	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	// ���[�g�V�O�l�`���̃V���A���C�Y
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature;

	// �p�C�v���C���X�e�[�g
	ID3D12PipelineState* pipelineState = nullptr;
	result = dev->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
#pragma endregion

	// �E�B���h�E�\��
	// �Q�[�����[�v
	while (true)
	{

#pragma region �E�B���h�E���b�Z�[�W
		win->Update();

		if (win->breakFlag)
		{
			break;
		}
#pragma endregion

#pragma region ���\�[�X�o���A
		// �o�b�N�o�b�t�@�̔ԍ����擾
		UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

		// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = backBuffers[bbIndex];
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region �`���
		// 2. �`���̕ύX
		// �����_�[�^�[�Q�[�g�r���[�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
#pragma endregion

#pragma region ��ʃN���A
		// 3. ��ʃN���A
		FLOAT clearColor[] = { bRed,bGreen,bBule,0.0f };
		cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
#pragma endregion

#pragma region �L�[�{�[�h
		// �L�[�{�[�h�̏��擾
		keyboad->Acquire();
		// �S�L�[�̓��͏�Ԃ�����
		BYTE key[256] = {};
		BYTE oldkey[256] = {};
		keyboad->GetDeviceState(sizeof(key), key);
		keyboad->GetDeviceState(sizeof(oldkey), oldkey);
		for (int i = 0; i < 256; ++i)
		{
			oldkey[i] = key[i];
		}

		if (key[DIK_SPACE]) {
			bRed = 1.0f;
			bGreen = 0.7f;
			bBule = 1.0f;
		}
		else {
			bRed = 0.1f;
			bGreen = 0.25f;
			bBule = 0.5f;
		}
#pragma endregion

#pragma region �`��
		// �`��R�}���h��������
		// �r���[�|�[�g�ݒ�R�}���h
		D3D12_VIEWPORT viewport{};
		/*D3D12_VIEWPORT viewport{};
		D3D12_VIEWPORT viewport{};
		D3D12_VIEWPORT viewport{};*/
		viewport.Width = win->window_width - 500;   // ����
		viewport.Height = win->window_height - 500; // �c��
		viewport.TopLeftX = 0;                 // ����x
		viewport.TopLeftY = 0;				   // ����y
		viewport.MinDepth = 0.0f;			   // �ŏ��[�x
		viewport.MaxDepth = 1.0f;			   // �ő�[�x

		// �r���[�|�[�g�ݒ�R�}���h���R�}���h���X�g�ɐς�
		cmdList->RSSetViewports(1, &viewport);

		// �V�U�[��`
		D3D12_RECT scissorRect{};
		scissorRect.left = 0;									// �؂蔲�����W��
		scissorRect.right = scissorRect.left + win->window_width;	// �؂蔲�����W�E
		scissorRect.top = 0;									// �؂蔲�����W��
		scissorRect.bottom = scissorRect.top +win-> window_height;	// �؂蔲�����W��

		// �V�U�[��`�ݒ�R�}���h���R�}���h���X�g�ɐς�
		cmdList->RSSetScissorRects(1, &scissorRect);

		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		cmdList->SetPipelineState(pipelineState);
		cmdList->SetGraphicsRootSignature(rootSignature);

		// �v���~�e�B�u�`��̐ݒ�R�}���h
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
		cmdList->IASetVertexBuffers(0, 1, &vbView);

		// �`��R�}���h
		cmdList->DrawInstanced(_countof(vertices), 1, 0, 0);
		// �`��R�}���h�����܂�
#pragma endregion

#pragma region ���\�[�X�o���A��߂�
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region �R�}���h�̃t���b�V��
		// ���߂̃N���[�Y
		result = cmdList->Close();
		assert(SUCCEEDED(result));
		// �R�}���h���X�g�̎��s
		ID3D12CommandList* cmdLists[] = { cmdList };
		cmdQueue->ExecuteCommandLists(1, cmdLists);

		// ��ʂɕ\������o�b�t�@���t���b�v(���\�̓���ւ�)
		result = swapChain->Present(1, 0);
		assert(SUCCEEDED(result));
#pragma endregion

#pragma region �R�}���h�����҂�
		// �R�}���h�̊�����҂�
		cmdQueue->Signal(fence, ++fenceVal);
		if (fence->GetCompletedValue() != fenceVal)
		{
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			fence->SetEventOnCompletion(fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}

		// �L���[���N���A
		result = cmdAllocater->Reset();
		assert(SUCCEEDED(result));
		// �ĂуR�}���h�𒙂߂鏀��
		result = cmdList->Reset(cmdAllocater, nullptr);
		assert(SUCCEEDED(result));
#pragma endregion
	}

	return 0;
}