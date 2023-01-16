#include "KDirectXCommon.h"
#include <thread>

// �Ή����x���̔z��
D3D_FEATURE_LEVEL levels[] = 
{
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0
};

void KDirectXCommon::Init()
{
	// FPS�Œ菉����
	InitFixFPS();

#ifdef _DEBUG
	// �f�o�b�O���C���[���I����
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(true);
	}
#endif
	// DXGI�t�@�N�g���[�̐���
	ComPtr<IDXGIFactory6> dxgiFactory;
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	// �A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter4>> adapters;

	// �����ɓ���̃A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter4> tmpAdapter;

	// �p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
		IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND; i++)
	{
		// ���I�z��ɒǉ�
		adapters.push_back(tmpAdapter);
	}

	// �Ó��ȃA�_�v�^��I�ʂ���
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;
		// �A�_�v�^�[�̏����擾
		adapters[i]->GetDesc3(&adapterDesc);

		// �\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// �f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter = adapters[i].Get();
			break;
		}
	}

	// �f�o�C�X����
	D3D_FEATURE_LEVEL featureLevel;
	for (size_t i = 0; i < _countof(levels); i++) {
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK) {
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}

	// �G���[���̃u���C�N
	tmpAdapter->Release();

	// �R�}���h�A���P�[�^�[����
	result = dev->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocater));
	assert(SUCCEEDED(result));

	// �R�}���h���X�g����
	result = dev->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocater.Get(), nullptr,
		IID_PPV_ARGS(&cmdList));
	assert(SUCCEEDED(result));

	// �R�}���h�L���[�𐶐�
	result = dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
	assert(SUCCEEDED(result));

	// �X���b�v�`�F�[���̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = KWinApp::GetWindowSizeW();
	swapChainDesc.Height = KWinApp::GetWindowSizeH();
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// �X���b�v�`�F�[���̐���
	ComPtr<IDXGISwapChain1> swapchain1;
	result = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(), KWinApp::GetHWND(), &swapChainDesc, nullptr, nullptr,
		&swapchain1);

	// IDXGISwapChain1�̃I�u�W�F�N�g��IDXGISwapChain4�ɕϊ�
	swapchain1.As(&swapChain);
	assert(SUCCEEDED(result));

	// RTV�p�f�X�N���v�^�q�[�v�̐���
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;
	result = dev->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
	assert(SUCCEEDED(result));

	backBuffers.resize(swapChainDesc.BufferCount);

	// �X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		// �X���b�v�`�F�[������o�b�t�@���擾
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		// �f�X�N���v�^�q�[�v�̃n���h�����擾
		rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		// �����\���ŃA�h���X�������
		rtvHandle.ptr += i * dev->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		// �����_�[�^�[�Q�[�g�r���[�̐ݒ�
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		// �V�F�[�_�[�̌v�Z���ʂ�SKGB�ɕϊ����ď�������
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// �����_�[�^�[�Q�[�g�r���[�̐���
		dev->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = KWinApp::GetWindowSizeW(); // �����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = KWinApp::GetWindowSizeH(); // �����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // �f�v�X�X�e���V��

	// �[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	// �[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g

	// �[�x�o�b�t�@
	result = dev->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));

	// �[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // �[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // �f�v�X�X�e���V���r���[

	//�@�[�x�r���[�p�q�[�v�쐬
	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	assert(SUCCEEDED(result));

	// �[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());

	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}

void KDirectXCommon::PreDraw()
{
#pragma region ���\�[�X�o���A
	// �o�b�N�o�b�t�@�̔ԍ����擾
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
	// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	cmdList->ResourceBarrier(1, &GetInstance()->barrierDesc);
#pragma endregion

#pragma region �`���
	// 2. �`���̕ύX
	// �����_�[�^�[�Q�[�g�r���[�̃n���h�����擾
	rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
#pragma endregion

#pragma region ��ʃN���A
	// 3. ��ʃN���A
	FLOAT clearColor[] = { 0.1f,0.25f,0.5f,0.0f };
	cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion

#pragma region �r���[�|�[�g�ݒ�R�}���h
	// �r���[�|�[�g�ݒ�R�}���h
	D3D12_VIEWPORT viewport{};
	viewport.Width = KWinApp::GetWindowSizeW();   // ����
	viewport.Height = KWinApp::GetWindowSizeH(); // �c��
	viewport.TopLeftX = 0;                 // ����x
	viewport.TopLeftY = 0;				   // ����y
	viewport.MinDepth = 0.0f;			   // �ŏ��[�x
	viewport.MaxDepth = 1.0f;			   // �ő�[�x
	// �r���[�|�[�g�ݒ�R�}���h���R�}���h���X�g�ɐς�
	cmdList->RSSetViewports(1, &viewport);
#pragma endregion

#pragma region �V�U�[��`�ݒ�
	// �V�U�[��`
	D3D12_RECT scissorRect{};
	scissorRect.left = 0;									// �؂蔲�����W��
	scissorRect.right = scissorRect.left + KWinApp::GetWindowSizeW();	// �؂蔲�����W�E
	scissorRect.top = 0;									// �؂蔲�����W��
	scissorRect.bottom = scissorRect.top + KWinApp::GetWindowSizeH();	// �؂蔲�����W��
	// �V�U�[��`�ݒ�R�}���h���R�}���h���X�g�ɐς�
	cmdList->RSSetScissorRects(1, &scissorRect);
#pragma endregion
}

void KDirectXCommon::PostDraw()
{
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
	ID3D12CommandList* cmdLists[] = { cmdList.Get() };
	cmdQueue->ExecuteCommandLists(1, cmdLists);
	// ��ʂɕ\������o�b�t�@���t���b�v(���\�̓���ւ�)
	result = swapChain->Present(1, 0);
	assert(SUCCEEDED(result));
#pragma endregion

#pragma region �R�}���h�����҂�
	// �R�}���h�̊�����҂�
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) 
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		if (event != 0)
		{
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
	}
	// �L���[���N���A
	result = cmdAllocater->Reset();
	assert(SUCCEEDED(result));
	// �ĂуR�}���h�𒙂߂鏀��
	result = cmdList->Reset(cmdAllocater.Get(), nullptr);
	assert(SUCCEEDED(result));
#pragma endregion
	// FPS�Œ�
	UpdateFixFPS();
}

void KDirectXCommon::InitFixFPS()
{
	// ���ݎ��Ԃ̋L�^
	reference_ = std::chrono::steady_clock::now();
}

void KDirectXCommon::UpdateFixFPS()
{
	// 1/60�b�̂҂�����̎���
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	// 1/60�b���킸���ɒZ������ 
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));
	// ���ݎ��Ԃ��擾����
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	// �O��L�^����̌o�ߎ��Ԃ��擾����
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);
	// 1/60�b(���킸���ɒZ������)�o���ĂȂ��ꍇ
	if (elapsed < kMinTime)
	{
		// 1/60�b�o�߂���܂Ŕ����ȃX���[�v���J��Ԃ�
		while (std::chrono::steady_clock::now() - reference_ < kMinTime)
		{
			// 1�}�C�N���b�X���[�v
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

	float nowTime = std::chrono::duration_cast<std::chrono::microseconds>
		(std::chrono::steady_clock::now() - reference_).count() / 1000000.0f;

	if (nowTime > 0)
	{
		fps = (fps * 0.99f) + (0.01f / nowTime);
	}
	// ���݂̎��Ԃ��L�^����
	reference_ = std::chrono::steady_clock::now();
}

KDirectXCommon* KDirectXCommon::GetInstance()
{
	static KDirectXCommon instance;
	return &instance;
}
