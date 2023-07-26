#include "ADXCommon.h"

#include <cassert>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"winmm.lib")

using namespace Microsoft::WRL;

ADXCommon* ADXCommon::S_currentInstance = nullptr;
const float ADXCommon::S_clearColor[4] = { 0.1f,0.8f,1.0f,0.0f };

//�Ή����x���̔z��
D3D_FEATURE_LEVEL levels[] =
{
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
};

void ADXCommon::InitializeFixFPS()
{
	reference_ = std::chrono::steady_clock::now();
}

void ADXCommon::UpdateFixFPS()
{
	//1/60�b�҂�����̎���
	const std::chrono::microseconds KMinTime(uint64_t(1000000.0f / 60.0f));
	//1/60�b���킸���ɒZ������
	const std::chrono::microseconds KMinCheckTime(uint64_t(1000000.0f / 65.0f));

	//���݂̎��Ԃ��擾����
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//�O��̋L�^����̌o�ߎ��Ԃ��擾����
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	//�o�ߎ��Ԃ�1/60�b����(�������Ԃɗ]�T������)
	if (elapsed < KMinTime)
	{
		//����\���グ��(�������Ȃ���Sleep�̐��x�̓K�^�K�^)
		timeBeginPeriod(1);
		while (std::chrono::steady_clock::now() - reference_ < KMinTime)
		{
			//1�}�C�N���b�X���[�v
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
		//�߂�
		timeEndPeriod(1);
	}
	//�v���I�����Ԃ��v���J�n���Ԃ�
	reference_ = std::chrono::steady_clock::now();
}

void ADXCommon::Initialize(ADXWindow* setWindow)
{
	assert(setWindow);

	adxwindow = setWindow;

	InitializeFixFPS();

	InitializeDevice();
	InitializeCommand();
	InitializeSwapChain();
	InitializeRenderTargetView();
	InitializeDepthBuffer();
	InitializeFence();

	S_currentInstance = this;
}

void ADXCommon::InitializeDevice()
{
	HRESULT result;

#ifdef _DEBUG
	//�f�o�b�O���C���[�L����
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif

	//�O���t�B�b�N�{�[�h�̃A�_�v�^���
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	ComPtr<IDXGIAdapter4> tmpAdapter = nullptr;

	for (uint32_t i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		adapters.push_back(tmpAdapter);
	}

	//�A�_�v�^�̑I��
	for (uint32_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesk;
		adapters[i]->GetDesc3(&adapterDesk);

		if (!(adapterDesk.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			tmpAdapter = adapters[i];
			break;
		}
	}

	//�f�o�C�X�̐���
	D3D_FEATURE_LEVEL featureLevel;

	for (uint32_t i = 0; i < _countof(levels); i++)
	{
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK)
		{
			featureLevel = levels[i];
			break;
		}
	}

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		D3D12_MESSAGE_ID denyIds[] =
		{
			D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE
		};

		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;

		infoQueue->PushStorageFilter(&filter);
	}
#endif
}

void ADXCommon::InitializeCommand()
{
	HRESULT result;

	//�R�}���h�A���P�[�^�[�𐶐�
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator));
	assert(SUCCEEDED(result));

	//�R�}���h���X�g�𐶐�
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(result));

	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//�R�}���h�L���[�̐���
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));
}

void ADXCommon::InitializeSwapChain()
{
	HRESULT result;

	//�X���b�v�`�F�[���̐ݒ�
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//IDXGISwapChain1��ComPtr��p��
	ComPtr<IDXGISwapChain1> swapchain1;
	//�X���b�v�`�F�[���̐���
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue.Get(), adxwindow->GetHwnd(), &swapChainDesc, nullptr, nullptr,
		&swapchain1);
	assert(SUCCEEDED(result));
	//��������IDXGISwapChain1�̃I�u�W�F�N�g��IDXGISwapChain4�ɕϊ�����
	swapchain1.As(&swapChain);
}

void ADXCommon::InitializeRenderTargetView()
{
	//�f�X�N���v�^�q�[�v�̐ݒ�
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;
	//�f�X�N���v�^�q�[�v�̐���
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

	//�o�b�N�o�b�t�@
	backBuffers.resize(swapChainDesc.BufferCount);

	//�X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	for (uint32_t i = 0; i < backBuffers.size(); i++)
	{
		swapChain->GetBuffer((uint32_t)i, IID_PPV_ARGS(&backBuffers[i]));
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		//�����_�[�^�[�Q�b�g�r���[�̐���
		device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}
}

void ADXCommon::InitializeDepthBuffer()
{
	HRESULT result;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = ADXWindow::S_window_width; //�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = ADXWindow::S_window_height; //�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT; //�[�x�l�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; //�f�v�X�X�e���V��

	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f; //�[�x�l1.0f�i�ő�l�j�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; //�[�x�l�t�H�[�}�b�g

	//���\�[�X����
	result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));

	//�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void ADXCommon::InitializeFence()
{
	HRESULT result;

	//�t�F���X�̐���
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}

void ADXCommon::ReturnRenderTarget()
{
	uint32_t bbIndex = swapChain->GetCurrentBackBufferIndex();

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
}

void ADXCommon::PreDraw()
{
	//DX�̉�ʍX�V����

	//�o�b�N�o�b�t�@�̔ԍ��擾
	uint32_t bbIndex = swapChain->GetCurrentBackBufferIndex();

	//���\�[�X�o���A�ŏ������݉\�ɕύX
	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	commandList->ResourceBarrier(1, &barrierDesc);

	//�`���̕ύX
	ReturnRenderTarget();
	
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);

	//�܂���ʂ�w�i�F�œh��ׂ�
	commandList->ClearRenderTargetView(rtvHandle, S_clearColor, 0, nullptr);

	// �r���[�|�[�g�ݒ�R�}���h
	D3D12_VIEWPORT viewport{};
	viewport.Width = (FLOAT)ADXWindow::S_window_width;
	viewport.Height = (FLOAT)ADXWindow::S_window_height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// �r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	commandList->RSSetViewports(1, &viewport);

	// �V�U�[��`
	D3D12_RECT scissorRect{};
	scissorRect.left = 0; // �؂蔲�����W��
	scissorRect.right = scissorRect.left + (LONG)ADXWindow::S_window_width; // �؂蔲�����W�E
	scissorRect.top = 0; // �؂蔲�����W��
	scissorRect.bottom = scissorRect.top + (LONG)ADXWindow::S_window_height; // �؂蔲�����W��
	// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	commandList->RSSetScissorRects(1, &scissorRect);

	S_currentInstance = this;
}

void ADXCommon::PostDraw()
{
	HRESULT result;
	
	ReturnRenderTarget();

	//�o�b�N�o�b�t�@�̔ԍ��擾
	uint32_t bbIndex = swapChain->GetCurrentBackBufferIndex();

	//���\�[�X�o�b�t�@��߂�
	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	commandList->ResourceBarrier(1, &barrierDesc);

	//���߂̃N���[�Y
	result = commandList->Close();
	assert(SUCCEEDED(result));
	//�R�}���h���X�g�̎��s
	ID3D12CommandList* commandLists[] = { commandList.Get() };
	commandQueue->ExecuteCommandLists(1, commandLists);

	//��ʂɕ\������o�b�t�@�̕\�������ւ���
	result = swapChain->Present(1, 0);
	assert(SUCCEEDED(result));

	//�R�}���h�̎��s������ҋ@
	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	UpdateFixFPS();

	//�L���[���N���A
	result = commandAllocator->Reset();
	assert(SUCCEEDED(result));
	//�ēx�R�}���h���X�g�����߂鏀��
	result = commandList->Reset(commandAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
}