#include <stdio.h>
#include <d3dcompiler.h>
#include "KWindow.h"
#include "KDirectInit.h"
#include "KInput.h"
#include "KDepth.h"
#include "KVertex.h"
#include "KVertexShader.h"
#include "KPixelShader.h"
#include "KTexture.h"
#include "KWorldTransform.h"
#include "ViewProjection.h"
#include "KMaterial.h"
#include "KGPlin.h"
#ifdef DEBUG
#include <iostream>
#endif
#pragma comment(lib, "d3dcompiler.Lib")

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
	KWindow win;
#pragma endregion

#pragma region DirectX������
	KDirectInit dx(win);
	KInput input(dx.result, win.window, win.handle);
#pragma endregion

#pragma region �`�揉����
#pragma region �[�x�o�b�t�@
	KDepth depth(dx, win);
#pragma endregion
	// ����
	float speed = 0.0f;
	float speedY = 0.0f;

#pragma region ���_�f�[�^
	KVertex vertex(dx);
#pragma endregion

#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�
	KGPlin Gpipeline(dx, dx.result, dx.dev, win.window_width, win.window_height, vertex);
#pragma endregion

#pragma region �e�N�X�`��������
	KTexture texture(dx, vertex);
#pragma endregion
	Vector3 center = { 0,0,1 };

#pragma endregion

	// �E�B���h�E�\��
	// �Q�[�����[�v
	while (true)
	{
#pragma region �E�B���h�E���b�Z�[�W
		if (win.breakFlag || input.IsPush(DIK_ESCAPE)) {
			break;
		}
#pragma endregion

		// �X�V
#pragma region DirectX���t���[������
#pragma region �E�B���h�E�A�b�v�f�[�g
		win.Update();
#pragma endregion

#pragma region input�A�b�v�f�[�g
		input.Update(dx.result);
#pragma endregion

#pragma region �L�[�{�[�h����
		// �w�i�F�ς�
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
		// �摜�F�ς�
		if (input.IsPush(DIK_1)) {
			Gpipeline.material->colorR = 1.0f;
			Gpipeline.material->colorG = 0.0f;
			Gpipeline.material->colorB = 1.0f;
		}
		else {
			Gpipeline.material->colorR = 1.0f;
			Gpipeline.material->colorG = 1.0f;
			Gpipeline.material->colorB = 1.0f;
		}
		// �}�`�c��]
		if (input.IsPush(DIK_F) ||
			input.IsPush(DIK_V)) {
			if (input.IsPush(DIK_F)) {
				Gpipeline.object3d->object3d[0].rot.x += 0.1f;
			}

			if (input.IsPush(DIK_V)) {
				Gpipeline.object3d->object3d[0].rot.x -= 0.1f;
			}
		}
		////�J�����ړ�
		if (input.IsPush(DIK_D) || input.IsPush(DIK_A) ||
			input.IsPush(DIK_W) || input.IsPush(DIK_S)) {
			if (input.IsPush(DIK_D)) {
				Gpipeline.viewProjection->angleX += XMConvertToRadians(1.0f);
			}
			else if (input.IsPush(DIK_A)) {
				Gpipeline.viewProjection->angleX -= XMConvertToRadians(1.0f);
			}

			if (input.IsPush(DIK_W)) {
				Gpipeline.viewProjection->angleY -= XMConvertToRadians(1.0f);
			}
			else if (input.IsPush(DIK_S)) {
				Gpipeline.viewProjection->angleY += XMConvertToRadians(1.0f);
			}

			// angle���W�A��y����]
			Gpipeline.viewProjection->eye.x = Gpipeline.viewProjection->lenZ * sinf(Gpipeline.viewProjection->angleX);
			Gpipeline.viewProjection->eye.y = Gpipeline.viewProjection->lenZ * sinf(Gpipeline.viewProjection->angleY);
			Gpipeline.viewProjection->eye.z = Gpipeline.viewProjection->lenZ * cosf(Gpipeline.viewProjection->angleX) * cosf(Gpipeline.viewProjection->angleY);
		}
		// �ړ�
		if (input.IsPush(DIK_UP) ||
			input.IsPush(DIK_DOWN) ||
			input.IsPush(DIK_RIGHT) ||
			input.IsPush(DIK_LEFT)) {
			if (input.IsPush(DIK_UP)) {
				speed = 1;
			}
			if (input.IsPush(DIK_DOWN)) {
				speed = -1;
			}
			if (input.IsPush(DIK_RIGHT)) {
				speedY = 1;
				//Gpipeline.object3d->object3d[0].rot.y -= 0.1f;
			}
			if (input.IsPush(DIK_LEFT)) {
				speedY = -1;
				//Gpipeline.object3d->object3d[0].rot.y += 0.1f;
			}
		}
		if (input.IsNPush(DIK_UP) && input.IsNPush(DIK_DOWN)) {
			speed = 0;
		}
		if (input.IsNPush(DIK_RIGHT) && input.IsNPush(DIK_LEFT)){
			speedY = 0;
		}
#pragma endregion
		Gpipeline.object3d->object3d[0].rot.z += XMConvertToRadians(5);

		// �O�x�N�g��
		Gpipeline.object3d->rotResult[0].x = sin(Gpipeline.object3d->object3d[0].rot.y) * center.z;
		Gpipeline.object3d->rotResult[0].z = cos(Gpipeline.object3d->object3d[0].rot.y) * center.z;
		// �ړ�
		Gpipeline.object3d->object3d[0].pos.x += (speedY);
		Gpipeline.object3d->object3d[0].pos.y += (speed);
#pragma region �摜�F�A�b�v�f�[�g
		Gpipeline.material->Update();
#pragma endregion

#pragma region �r���[�̃A�b�v�f�[�g
		Gpipeline.viewProjection->Update(win.window_width, win.window_height);
#pragma endregion

#pragma region 3D�I�u�W�F�N�g�̃A�b�v�f�[�g
		Gpipeline.object3d->Update(Gpipeline.viewProjection->matView, Gpipeline.viewProjection->matProjection);
#pragma endregion

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
		// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depth.dsvHeap->GetCPUDescriptorHandleForHeapStart();
		dx.cmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
#pragma endregion

#pragma region ��ʃN���A
		// 3. ��ʃN���A
		FLOAT clearColor[] = { dx.bRed,dx.bGreen,dx.bBule,0.0f };
		dx.cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		dx.cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion

#pragma region �`��
		// �`��R�}���h��������
#pragma region �r���[�|�[�g�ݒ�R�}���h
		// �r���[�|�[�g�ݒ�R�}���h
		D3D12_VIEWPORT viewport{};
		viewport.Width = win.window_width;   // ����
		viewport.Height = win.window_height; // �c��
		viewport.TopLeftX = 0;                 // ����x
		viewport.TopLeftY = 0;				   // ����y
		viewport.MinDepth = 0.0f;			   // �ŏ��[�x
		viewport.MaxDepth = 1.0f;			   // �ő�[�x
		// �r���[�|�[�g�ݒ�R�}���h���R�}���h���X�g�ɐς�
		dx.cmdList->RSSetViewports(1, &viewport);
#pragma endregion
#pragma region �V�U�[��`�ݒ�
		// �V�U�[��`
		D3D12_RECT scissorRect{};
		scissorRect.left = 0;									// �؂蔲�����W��
		scissorRect.right = scissorRect.left + win.window_width;	// �؂蔲�����W�E
		scissorRect.top = 0;									// �؂蔲�����W��
		scissorRect.bottom = scissorRect.top + win.window_height;	// �؂蔲�����W��
		// �V�U�[��`�ݒ�R�}���h���R�}���h���X�g�ɐς�
		dx.cmdList->RSSetScissorRects(1, &scissorRect);
#pragma endregion
#pragma region �p�C�v���C���X�e�[�g�ݒ�
		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		dx.cmdList->SetPipelineState(Gpipeline.pipelineState);
		dx.cmdList->SetGraphicsRootSignature(Gpipeline.rootSignature);
#pragma endregion
#pragma region �v���~�e�B�u�`��
		// �v���~�e�B�u�`��̐ݒ�R�}���h
		dx.cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion
		// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
		dx.cmdList->IASetIndexBuffer(&vertex.ibView);
#pragma region ���_�o�b�t�@�r���[�̐ݒ�R�}���h
		// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
		dx.cmdList->IASetVertexBuffers(0, 1, &vertex.vbView);
#pragma endregion
		// CBV
		dx.cmdList->SetGraphicsRootConstantBufferView(0, Gpipeline.material->constBufferMaterial->GetGPUVirtualAddress());
		// SRV
		dx.cmdList->SetDescriptorHeaps(1, &texture.srvHeap);
		// �擪�n���h�����擾
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = texture.srvHeap->GetGPUDescriptorHandleForHeapStart();
		// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃ̐ݒ�
		dx.cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
#pragma region �`��R�}���h
		// �`��R�}���h
		Gpipeline.object3d->Draw(dx.cmdList, vertex.vbView, vertex.ibView, _countof(indices));
#pragma endregion
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
		if (dx.fence->GetCompletedValue() != dx.fenceVal) {
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