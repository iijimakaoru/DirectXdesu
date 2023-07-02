#include "KDirectXCommon.h"
#include "KWinApp.h"
#include <thread>

KDirectXCommon* KDirectXCommon::directXCommon_ = nullptr;

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
	EnbleDebugLayer();
#endif

	//DirectX12�֘A������
	if (FAILED(InitDXGIDevice()))
	{
		assert(0);
		return;
	}

#ifdef _DEBUG
	EnbleInfoQueue();
#endif

	if (FAILED(InitCommand()))
	{
		assert(0);
		return;
	}
	if (FAILED(CreateSwapChain()))
	{
		assert(0);
		return;
	}
	if (FAILED(CreateDepthBuffer()))
	{
		assert(0);
		return;
	}
	if (FAILED(CreateFinalRenderTarget()))
	{
		assert(0);
		return;
	}
	if (FAILED(CreateFence()))
	{
		assert(0);
		return;
	}

	//�f�X�N�v���^�q�[�v����
	srvHeap = std::make_unique<KDescriptorHeap>();
	srvHeap->Initialize();
}

void KDirectXCommon::PreDraw()
{
	//1�o�b�N�o�b�t�@�ԍ����擾
	bbIndex = swapChain->GetCurrentBackBufferIndex();
	//�������݉\�ɕύX

	backBuffers[bbIndex]->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET, cmdList.Get());

	cmdList->OMSetRenderTargets(1, &backBuffers[bbIndex]->GetHandle(), false, &depthBuff->GetHandle());

	//3��ʃN���A
	cmdList->ClearRenderTargetView(backBuffers[bbIndex]->GetHandle(), clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(depthBuff->GetHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	//�r���[�|�[�g�ݒ�
	viewport.Width = (FLOAT)KWinApp::GetInstance()->GetWindowSizeW();
	viewport.Height = (FLOAT)KWinApp::GetInstance()->GetWindowSizeH();
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// �r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	cmdList->RSSetViewports(1, &viewport);

	// �V�U�[��`�ݒ�
	scissorRect.left = 0; // �؂蔲�����W��
	scissorRect.right = scissorRect.left + KWinApp::GetInstance()->GetWindowSizeW(); // �؂蔲�����W�E
	scissorRect.top = 0; // �؂蔲�����W��
	scissorRect.bottom = scissorRect.top + KWinApp::GetInstance()->GetWindowSizeH(); // �؂蔲�����W��

	// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	cmdList->RSSetScissorRects(1, &scissorRect);
}

void KDirectXCommon::PostDraw()
{
	//5���ɖ߂�
	backBuffers[bbIndex]->Transition(D3D12_RESOURCE_STATE_PRESENT, cmdList.Get());

	DeleteCommand();
	// FPS�Œ�
	UpdateFixFPS();
}

void KDirectXCommon::DeleteCommand()
{
	//���߂̃N���[�Y
	result = cmdList->Close();
	assert(SUCCEEDED(result));
	//�R�}���h���X�g�̎��s
	ID3D12CommandList* commandListts[] = { cmdList.Get() };
	cmdQueue->ExecuteCommandLists(1, commandListts);

	//�t���b�v
	result = swapChain->Present(1, 0);
	assert(SUCCEEDED(result));

	//�R�}���h���s������҂�
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

	//�L���[���N���A
	result = cmdAllocater->Reset();
	assert(SUCCEEDED(result));
	//�R�}���h���X�g�𒙂߂鏀��
	if (cmdList != 0)
	{
		result = cmdList->Reset(cmdAllocater.Get(), nullptr);
		assert(SUCCEEDED(result));
	}
	else
	{
		assert(SUCCEEDED(0));
	}
}

void KDirectXCommon::Destroy()
{
	delete directXCommon_;
}

void KDirectXCommon::SetBackScreenColor(float R, float G, float B, float A)
{
	clearColor[0] = R;
	clearColor[1] = G;
	clearColor[2] = B;
	clearColor[3] = A;
}

KDescriptorHeap* KDirectXCommon::GetSRVDescriptorHeap()
{
	return srvHeap.get();
}

KRtvDescriptorHeap* KDirectXCommon::GetRTVDescriptorHeap()
{
	return rtvHeap.get();
}

KDsvDescriptorHeap* KDirectXCommon::GetDsvDescriptorHrap()
{
	return dsvHeap.get();
}

void KDirectXCommon::Transition(ID3D12Resource* resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
{
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(resource, beforeState, afterState);
	cmdList->ResourceBarrier(1, &barrier);
}

size_t KDirectXCommon::GetBackBufferCount() const
{
	return backBuffers.size();
}

HRESULT KDirectXCommon::InitDXGIDevice()
{
	//DXGI�t�@�N�g���[
	result = CreateDXGIFactory(IID_PPV_ARGS(dxgiFactory.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		return result;
	}

	//�A�_�v�^�[�񋓗p
	std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter4>> adapters;
	//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	Microsoft::WRL::ComPtr<IDXGIAdapter4> tmpAdapter;

	//�p�t�H�[�}���X�������̂�����S�ė�
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(tmpAdapter.ReleaseAndGetAddressOf())) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);
	}

	// �Ó��ȃA�_�v�^��I�ʂ���
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// �A�_�v�^�[�̏����擾����
		adapters[i]->GetDesc3(&adapterDesc);

		// �\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// �f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter = adapters[i];
			break;
		}
	}

	//Direct3D�f�o�C�X�̏�����
	D3D_FEATURE_LEVEL featureLevel;

	for (size_t i = 0; i < _countof(levels); i++) {
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(dev.GetAddressOf()));
		if (result == S_OK) {
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}

	return result;
}

HRESULT KDirectXCommon::CreateFinalRenderTarget()
{
	//�f�X�N���v�^�q�[�v�̐���
	rtvHeap = std::make_unique<KRtvDescriptorHeap>();
	rtvHeap->Initialize();

	//�o�b�N�o�b�t�@
	backBuffers.resize(swapChainDesc.BufferCount);

	//�X���b�v�`�F�[���̃o�b�t�@������
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		backBuffers[i] = std::make_unique<KRenderTargetBuffer>(dev.Get(), rtvHeap.get());

		//����
		backBuffers[i]->Create(swapChain.Get(), static_cast<UINT>(i));
	}

	return S_OK;
}

HRESULT KDirectXCommon::CreateSwapChain()
{
	swapChainDesc.Width = KWinApp::GetInstance()->GetWindowSizeW();//����
	swapChainDesc.Height = KWinApp::GetInstance()->GetWindowSizeH();//�c��
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//�F���̏���
	swapChainDesc.SampleDesc.Count = 1;//�}���`�T���v���Ȃ�
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;//�o�b�N�o�b�t�@�p
	swapChainDesc.BufferCount = 2;//�o�b�t�@��2
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//�t���b�v��͔j��
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//����
	if (cmdQueue != 0)
	{
		Microsoft::WRL::ComPtr<IDXGISwapChain1> tmpSwapChain;
		result = dxgiFactory->CreateSwapChainForHwnd(cmdQueue.Get(), KWinApp::GetInstance()->GetHWND(), &swapChainDesc, nullptr, nullptr, tmpSwapChain.ReleaseAndGetAddressOf());
		tmpSwapChain.As(&swapChain);

		if (FAILED(result))
		{
			return result;
		}
	}
	else
	{
		assert(SUCCEEDED(0));
	}

	return result;
}

HRESULT KDirectXCommon::InitCommand()
{
	//�R�}���h�A���P�[�^�𐶐�
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(cmdAllocater.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		return result;
	}

	//�R�}���h���X�g�𐶐�
	if (cmdAllocater != 0)
	{
		result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocater.Get(), nullptr, IID_PPV_ARGS(cmdList.ReleaseAndGetAddressOf()));
		if (FAILED(result))
		{
			return result;
		}
	}
	else
	{
		assert(SUCCEEDED(0));
	}

	//�R�}���h�L���[�̐ݒ聕����
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	result = dev->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(cmdQueue.ReleaseAndGetAddressOf()));

	if (FAILED(result))
	{
		return result;
	}

	return result;
}

HRESULT KDirectXCommon::CreateFence()
{
	//�t�F���X�̐���
	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf()));

	return result;
}

HRESULT KDirectXCommon::CreateDepthBuffer()
{
	//�f�X�N�v���^�q�[�v����
	dsvHeap = std::make_unique<KDsvDescriptorHeap>();
	dsvHeap->Initialize();

	//�[�x�o�b�t�@����
	depthBuff = std::make_unique<KDepthStencilBuffer>(dev.Get(), dsvHeap.get());
	depthBuff->Create(KWinApp::GetInstance()->GetWindowSizeW(), KWinApp::GetInstance()->GetWindowSizeH(), DXGI_FORMAT_D32_FLOAT);

	return result;
}

void KDirectXCommon::EnbleDebugLayer()
{
	Microsoft::WRL::ComPtr <ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.ReleaseAndGetAddressOf()))))
	{
		debugController->EnableDebugLayer();
		//debugController->SetEnableGPUBasedValidation(true);
	}
}

void KDirectXCommon::EnbleInfoQueue()
{
	Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
	result = dev->QueryInterface(IID_PPV_ARGS(&infoQueue));
	if (SUCCEEDED(result))
	{
		// ���o��
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		// ����
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		// �x��
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		//�}������G���[
		D3D12_MESSAGE_ID denyIds[] =
		{
			/*
			*windows11�ł�DXGI�f�o�b�O���C���[��DX12�f�o�b�O���C���[
			*���ݍ�p�o�O�ɂ��G���[���b�Z�[�W
			*/
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE

		};
		//�}������\�����x��
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//�w�肵���G���[�̕\����}������
		infoQueue->PushStorageFilter(&filter);
	}
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
	if (directXCommon_ == nullptr)
	{
		directXCommon_ = new KDirectXCommon();
	}

	return directXCommon_;
}

void KDirectXCommon::ResourceTransition(ID3D12Resource* resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
{
	GetInstance()->Transition(resource, beforeState, afterState);
}
