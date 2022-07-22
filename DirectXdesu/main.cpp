#include <stdio.h>
#include <d3dcompiler.h>
#include "KWindow.h"
#include "KDirectInit.h"
#include "KInput.h"
#include "KDepth.h"
#include "KVertex.h"
#include "KTexture.h"
#include "KWorldTransform.h"
#include "ViewProjection.h"
#include "KMaterial.h"
#include "KGPlin.h"
#include "KModel.h"
#ifdef DEBUG
#include <iostream>
#endif
#pragma comment(lib, "d3dcompiler.Lib")

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
	// �f�o�b�O���C���[���I����
	ID3D12Debug* debugController;
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
	KInput input(win.window, win.handle);
#pragma endregion

#pragma region �`�揉����
#pragma region �[�x�o�b�t�@
	KDepth depth(dx.dev, win);
#pragma endregion
	// ����
	float speed = 1.0f;
#pragma region ���f��
	KModel triangle = Triangle();
	triangle.CreateModel(dx.dev);
	KModel cube = Cube();
	cube.CreateModel(dx.dev);
#pragma endregion
#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�
	KGPlin Gpipeline(dx.dev, win.window_width, win.window_height);

	// 3D�I�u�W�F�N�g
	KWorldTransform* object3d[ObjectNum] = { nullptr };
#pragma region 3D�I�u�W�F�N�g������
	for (int i = 0; i < ObjectNum; i++) {
		object3d[i] = new KWorldTransform(dx.dev);
	}
#pragma endregion
#pragma endregion
#pragma region �e�N�X�`��������
	KTexture texture(dx.dev);
#pragma endregion
	Vector3 center = { 0,0,1 };

	float rSpeed = -0.02f;
	float gSpeed = 0.02f;
	float bSpeed = -0.02f;
	float aSpeed = -0.02f;

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
		input.Update();
#pragma endregion

#pragma region �L�[�{�[�h����
		// �w�i�F�ς�
		if (input.IsPush(DIK_SPACE)) {
			dx.bRed = 0.5f;
			dx.bGreen = 0.5f;
			dx.bBule = 0.5f;
		}
		else {
			dx.bRed = 0.1f;
			dx.bGreen = 0.25f;
			dx.bBule = 0.5f;
		}
		// �摜�F�ς�
		if (Gpipeline.material->colorR <= 0 || Gpipeline.material->colorR >= 1) {
			rSpeed *= -1;
		}
		if (Gpipeline.material->colorG <= 0 || Gpipeline.material->colorG >= 1) {
			gSpeed *= -1;
		}
		if (Gpipeline.material->colorB <= 0 || Gpipeline.material->colorB >= 1) {
			bSpeed *= -1;
		}
		if (Gpipeline.material->colorA <= 0 || Gpipeline.material->colorA >= 1) {
			aSpeed *= -1;
		}
		Gpipeline.material->colorR += rSpeed;
		Gpipeline.material->colorG += gSpeed;
		Gpipeline.material->colorB += bSpeed;
		if (input.IsPush(DIK_X)) {
			Gpipeline.material->colorA += aSpeed;
		}
		// �}�`�c��]
		if (input.IsPush(DIK_C) ||
			input.IsPush(DIK_V)) {
			if (input.IsPush(DIK_C)) {
				object3d[0]->transform.rot.z += 0.1f;
			}

			if (input.IsPush(DIK_V)) {
				object3d[0]->transform.rot.z -= 0.1f;
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
		//// ����]
		//if (input.IsPush(DIK_RIGHT) ||
		//	input.IsPush(DIK_LEFT)) {
		//	if (input.IsPush(DIK_RIGHT)) {
		//		object3d[0]->transform.rot.y -= 0.1f;
		//	}
		//	if (input.IsPush(DIK_LEFT)) {
		//		object3d[0]->transform.rot.y += 0.1f;
		//	}
		//}
		//// �ړ�
		//if (input.IsPush(DIK_UP) ||
		//	input.IsPush(DIK_DOWN)) {
		//	if (input.IsPush(DIK_UP)) {
		//		speed = 1;
		//	}
		//	if (input.IsPush(DIK_DOWN)) {
		//		speed = -1;
		//	}
		//}
		//else {
		//	speed = 0;
		//}
		// �O�x�N�g��
		object3d[0]->rotResult.x = sin(object3d[0]->transform.rot.y) * center.z;
		object3d[0]->rotResult.z = cos(object3d[0]->transform.rot.y) * center.z;
		// �ړ�
		/*object3d[0]->transform.pos.x += (speed)*object3d[0]->rotResult.x;
		object3d[0]->transform.pos.z += (speed)*object3d[0]->rotResult.z;*/
		object3d[0]->transform.pos.x += (input.IsPush(DIK_RIGHT) - input.IsPush(DIK_LEFT)) * speed;
		object3d[0]->transform.pos.z += (input.IsPush(DIK_UP) - input.IsPush(DIK_DOWN)) * speed;
#pragma endregion

#pragma region �摜�F�A�b�v�f�[�g
		Gpipeline.material->Update();
#pragma endregion

#pragma region �r���[�̃A�b�v�f�[�g
		Gpipeline.viewProjection->Update(win.window_width, win.window_height);
#pragma endregion

#pragma region 3D�I�u�W�F�N�g�̃A�b�v�f�[�g
		object3d[0]->Update(Gpipeline.viewProjection->matView, Gpipeline.viewProjection->matProjection);
#pragma endregion

#pragma endregion

		// �`��
		object3d[0]->SetModel(&triangle);
		object3d[1]->SetModel(&cube);

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
		// CBV
		dx.cmdList->SetGraphicsRootConstantBufferView(0, Gpipeline.material->constBufferMaterial->GetGPUVirtualAddress());
		// SRV
		dx.cmdList->SetDescriptorHeaps(1, &texture.srvHeap);
		// �擪�n���h�����擾
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = texture.srvHeap->GetGPUDescriptorHandleForHeapStart();
		srvGpuHandle.ptr += texture.incrementSize * 0;
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle2 = texture.srvHeap->GetGPUDescriptorHandleForHeapStart();
		srvGpuHandle2.ptr += texture.incrementSize * 1;
		// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃ̐ݒ�
		if (input.IsPush(DIK_SPACE)) {
			dx.cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle2);
		}
		else {
			dx.cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
		}
		
#pragma region �`��R�}���h
		// �`��R�}���h
		for (int i = 0; i < ObjectNum; i++) {
			object3d[i]->Draw(dx.cmdList);
		}
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