#include "KDirectXCommon.h"

KDirectXCommon::KDirectXCommon(KWinApp window) {
	bRed = 0.1f;
	bGreen = 0.25f;
	bBule = 0.5f;

	DXGIFactory();

	Adapter();

	CommandList();

	CommandQueue();

	SwapChain(window);

	Descriptor();

	BackBuffer();

	Fence();
}

void KDirectXCommon::PreDraw(ID3D12DescriptorHeap* dsvHeap, int window_width, int window_height)
{
#pragma region ���\�[�X�o���A
	// �o�b�N�o�b�t�@�̔ԍ����擾
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
	// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region �`���
	// 2. �`���̕ύX
	// �����_�[�^�[�Q�[�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
#pragma endregion

#pragma region ��ʃN���A
	// 3. ��ʃN���A
	FLOAT clearColor[] = { bRed,bGreen,bBule,0.0f };
	cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion

#pragma region �r���[�|�[�g�ݒ�R�}���h
	// �r���[�|�[�g�ݒ�R�}���h
	D3D12_VIEWPORT viewport{};
	viewport.Width = window_width;   // ����
	viewport.Height = window_height; // �c��
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
	scissorRect.right = scissorRect.left + window_width;	// �؂蔲�����W�E
	scissorRect.top = 0;									// �؂蔲�����W��
	scissorRect.bottom = scissorRect.top + window_height;	// �؂蔲�����W��
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
	CmdFlash();
#pragma endregion
#pragma region �R�}���h�����҂�
	CmdClear();
#pragma endregion
}

void KDirectXCommon::CmdFlash()
{
	// ���߂̃N���[�Y
	result = cmdList->Close();
	assert(SUCCEEDED(result));
	// �R�}���h���X�g�̎��s
	ID3D12CommandList* cmdLists[] = { cmdList };
	cmdQueue->ExecuteCommandLists(1, cmdLists);
	// ��ʂɕ\������o�b�t�@���t���b�v(���\�̓���ւ�)
	result = swapChain->Present(1, 0);
	assert(SUCCEEDED(result));
}

void KDirectXCommon::CmdClear()
{
	// �R�}���h�̊�����҂�
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	// �L���[���N���A
	result = cmdAllocater->Reset();
	assert(SUCCEEDED(result));
	// �ĂуR�}���h�𒙂߂鏀��
	result = cmdList->Reset(cmdAllocater.Get(), nullptr);
	assert(SUCCEEDED(result));
}

void KDirectXCommon::DXGIFactory() {
	// DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));
}

void KDirectXCommon::Adapter() {
	// �A�_�v�^�[�̗񋓗p
	std::vector<IDXGIAdapter4*> adapters;

	// �����ɓ���̃A�_�v�^�[�I�u�W�F�N�g������
	IDXGIAdapter4* tmpAdapter = nullptr;

	// �p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
		IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND; i++) {
		// ���I�z��ɒǉ�
		adapters.push_back(tmpAdapter);
	}

	// �Ó��ȃA�_�v�^��I�ʂ���
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// �A�_�v�^�[�̏����擾
		adapters[i]->GetDesc3(&adapterDesc);

		// �\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// �f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter = adapters[i];
			break;
		}
	}

	Device(tmpAdapter);

	tmpAdapter->Release();
}

void KDirectXCommon::Device(IDXGIAdapter4* tmpAdapter) {
	for (size_t i = 0; i < _countof(levels); i++) {
		result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK) {
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}
}

void KDirectXCommon::CommandList() {
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
}

void KDirectXCommon::CommandQueue() {
	// �R�}���h�L���[�𐶐�
	result = dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
	assert(SUCCEEDED(result));
}

void KDirectXCommon::SwapChain(KWinApp window) {
	swapChainDesc.Width = window.window_width;
	swapChainDesc.Height = window.window_height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr<IDXGISwapChain1> swapchain1;
	// �X���b�v�`�F�[���̐���
	result = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(), window.hwnd, &swapChainDesc, nullptr, nullptr,
		&swapchain1);

	swapchain1.As(&swapChain);

	assert(SUCCEEDED(result));
}

void KDirectXCommon::Descriptor() {
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;
	// �f�X�N���v�^�q�[�v�̐���
	dev->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
}

void KDirectXCommon::BackBuffer() {
	backBuffers.resize(swapChainDesc.BufferCount);
	// �X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		// �X���b�v�`�F�[������o�b�t�@���擾
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		// �f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
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
}

void KDirectXCommon::Fence() {
	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}