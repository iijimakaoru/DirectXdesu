#include <stdio.h>
#include <d3dcompiler.h>
#include "KWindow.h"
#include "KDirectInit.h"
#include "KInput.h"
#include "KDepth.h"
#include "KTexture.h"
#include "KWorldTransform.h"
#include "ViewProjection.h"
#include "KGPlin.h"
#include "KModel.h"
#ifdef _DEBUG
#include <iostream>
#endif
#pragma comment(lib, "d3dcompiler.Lib")

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
	// �f�o�b�O���C���[���I����
	ID3D12Debug1* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(true);
	}
#endif

	HRESULT result;

#pragma region �E�B���h�E
	KWindow win;
#pragma endregion

#pragma region DirectX������
	KDirectInit dx(win);
	//#ifdef _DEBUG
	//	ID3D12InfoQueue* infoQueue;
	//	if (SUCCEEDED(dx.dev->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
	//		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
	//		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	//		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
	//		infoQueue->Release();
	//	}
	//#endif // _DEBUG
	KInput input(win.window, win.handle);
#pragma endregion

#pragma region �`�揉����
#pragma region �[�x�o�b�t�@
	KDepth depth(dx.SetDev().Get(), win);
#pragma endregion
	// ����
	float speed = 1.0f;
#pragma region ���f��
	KModel triangle = Triangle();
	triangle.CreateModel(dx.SetDev().Get());
	KModel cube = Cube();
	cube.CreateModel(dx.SetDev().Get());
	KModel line = Line();
	line.CreateModel(dx.SetDev().Get());
#pragma endregion

#pragma region �e�N�X�`��������
	const wchar_t* msg = L"Resources/mario.jpg";
	const wchar_t* msg2 = L"Resources/���[�����.jpg";
	const wchar_t* msg3 = L"Resources/haikei.jpg";
	const wchar_t* msg4 = L"Resources/kitanai.jpg";
	KTexture texture(dx.SetDev().Get(), msg, msg3);
	KTexture texture2(dx.SetDev().Get(), msg2, msg4);
#pragma endregion

#pragma region �O���t�B�b�N�X�p�C�v���C���ݒ�
	KGPlin Gpipeline(dx.SetDev().Get());
#pragma region 3D�I�u�W�F�N�g������
	const int ObjectNum = 2;
	const int LineNum = 6;
	// 3D�I�u�W�F�N�g
	KWorldTransform object3d[ObjectNum];
	//KWorldTransform lineObject[LineNum];
	for (int i = 0; i < ObjectNum; i++) {
		object3d[i].Initialize(*dx.SetDev().Get());
		if (i > 0) {
			object3d[i].material->colorR = object3d[i].material->colorG = object3d[i].material->colorB = 1.0f;
		}
	}
	object3d[0].SetModel(&cube);
	object3d[0].SetTexture(&texture);
	object3d[1].SetModel(&cube);
	object3d[1].SetTexture(&texture2);
	/*for (int i = 0; i < LineNum; i++) {
		lineObject[i].Initialize(*dx.dev);
		lineObject[i].SetModel(&line);
	}
	lineObject[1].transform.pos.x = lineObject[0].transform.pos.x + 20;
	lineObject[2].transform.pos.x = lineObject[0].transform.pos.x - 20;
	lineObject[3].transform.rot.y = XMConvertToRadians(90);
	lineObject[4].transform.rot.y = XMConvertToRadians(90);
	lineObject[4].transform.pos.z = lineObject[3].transform.pos.z + 20;
	lineObject[5].transform.rot.y = XMConvertToRadians(90);
	lineObject[5].transform.pos.z = lineObject[3].transform.pos.z - 20;*/
#pragma endregion

#pragma region �r���[
	// �r���[�v���W�F�N�V����
	ViewProjection* viewProjection;
	viewProjection = new ViewProjection(win.window_width, win.window_height);
#pragma endregion

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
		if (object3d[0].material->colorR <= 0 || object3d[0].material->colorR >= 1) {
			rSpeed *= -1;
		}
		if (object3d[0].material->colorG <= 0 || object3d[0].material->colorG >= 1) {
			gSpeed *= -1;
		}
		if (object3d[0].material->colorB <= 0 || object3d[0].material->colorB >= 1) {
			bSpeed *= -1;
		}
		if (object3d[0].material->colorA <= 0 || object3d[0].material->colorA >= 1) {
			aSpeed *= -1;
		}
		object3d[0].material->colorR += rSpeed;
		object3d[0].material->colorG += gSpeed;
		object3d[0].material->colorB += bSpeed;
		if (input.IsPush(DIK_X)) {
			object3d[0].material->colorA += aSpeed;
		}
		// �}�`�c��]
		if (input.IsPush(DIK_C) ||
			input.IsPush(DIK_V)) {
			if (input.IsPush(DIK_C)) {
				object3d[0].transform.rot.z += 0.1f;
			}

			if (input.IsPush(DIK_V)) {
				object3d[0].transform.rot.z -= 0.1f;
			}
		}
		////�J�����ړ�
		if (input.IsPush(DIK_D) || input.IsPush(DIK_A) ||
			input.IsPush(DIK_W) || input.IsPush(DIK_S)) {
			if (input.IsPush(DIK_D)) {
				viewProjection->angleX += XMConvertToRadians(1.0f);
			}
			else if (input.IsPush(DIK_A)) {
				viewProjection->angleX -= XMConvertToRadians(1.0f);
			}
			if (input.IsPush(DIK_W)) {
				viewProjection->angleY -= XMConvertToRadians(1.0f);
			}
			else if (input.IsPush(DIK_S)) {
				viewProjection->angleY += XMConvertToRadians(1.0f);
			}
			// angle���W�A��y����]
			viewProjection->eye.x = viewProjection->lenZ * sinf(viewProjection->angleX);
			viewProjection->eye.y = viewProjection->lenZ * sinf(viewProjection->angleY);
			viewProjection->eye.z = viewProjection->lenZ * cosf(viewProjection->angleX) * cosf(viewProjection->angleY);
		}
		// ����]
		if (input.IsPush(DIK_RIGHT) ||
			input.IsPush(DIK_LEFT)) {
			if (input.IsPush(DIK_RIGHT)) {
				object3d[0].transform.rot.y -= 0.1f;
			}
			if (input.IsPush(DIK_LEFT)) {
				object3d[0].transform.rot.y += 0.1f;
			}
		}
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
		object3d[0].rotResult.x = sin(object3d[0].transform.rot.y) * center.z;
		object3d[0].rotResult.z = cos(object3d[0].transform.rot.y) * center.z;
		// �ړ�
		/*object3d[0]->transform.pos.x += (speed)*object3d[0]->rotResult.x;
		object3d[0]->transform.pos.z += (speed)*object3d[0]->rotResult.z;*/
		object3d[0].transform.pos.x += (input.IsPush(DIK_RIGHT) - input.IsPush(DIK_LEFT)) * speed;
		object3d[0].transform.pos.z += (input.IsPush(DIK_UP) - input.IsPush(DIK_DOWN)) * speed;

		object3d[1].transform.pos.x = object3d[0].transform.pos.x + 15;
		object3d[1].transform.pos.z = object3d[0].transform.pos.z;
		object3d[1].transform.pos.y = object3d[0].transform.pos.y + 10;

		object3d[1].transform.rot = object3d[0].transform.rot;
#pragma endregion

#pragma region �摜�F�A�b�v�f�[�g
		//material->Update();
#pragma endregion

#pragma region �r���[�̃A�b�v�f�[�g
		viewProjection->Update(win.window_width, win.window_height);
#pragma endregion

#pragma region 3D�I�u�W�F�N�g�̃A�b�v�f�[�g
		for (int i = 0; i < ObjectNum; i++) {
			object3d[i].Update(viewProjection->matView, viewProjection->matProjection);
		}
		/*for (int i = 0; i < LineNum; i++) {
			lineObject[i].Update(viewProjection->matView, viewProjection->matProjection);
		}*/
#pragma endregion

#pragma endregion

		// �`��
#pragma region ���\�[�X�o���A
		// �o�b�N�o�b�t�@�̔ԍ����擾
		UINT bbIndex = dx.SetSChain()->GetCurrentBackBufferIndex();
		// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = dx.SetBackBuffers()[bbIndex].Get();
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		dx.SetCmdlist()->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region �`���
		// 2. �`���̕ύX
		// �����_�[�^�[�Q�[�g�r���[�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx.SetRtvHeap()->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * dx.SetDev()->GetDescriptorHandleIncrementSize(dx.SetRtvHeapDesc().Type);
		// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depth.dsvHeap->GetCPUDescriptorHandleForHeapStart();
		dx.SetCmdlist()->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
#pragma endregion

#pragma region ��ʃN���A
		// 3. ��ʃN���A
		FLOAT clearColor[] = { dx.bRed,dx.bGreen,dx.bBule,0.0f };
		dx.SetCmdlist()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		dx.SetCmdlist()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
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
		dx.SetCmdlist()->RSSetViewports(1, &viewport);
#pragma endregion
#pragma region �V�U�[��`�ݒ�
		// �V�U�[��`
		D3D12_RECT scissorRect{};
		scissorRect.left = 0;									// �؂蔲�����W��
		scissorRect.right = scissorRect.left + win.window_width;	// �؂蔲�����W�E
		scissorRect.top = 0;									// �؂蔲�����W��
		scissorRect.bottom = scissorRect.top + win.window_height;	// �؂蔲�����W��
		// �V�U�[��`�ݒ�R�}���h���R�}���h���X�g�ɐς�
		dx.SetCmdlist()->RSSetScissorRects(1, &scissorRect);
#pragma endregion
#pragma region �p�C�v���C���X�e�[�g�ݒ�
		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		dx.SetCmdlist()->SetPipelineState(Gpipeline.pipelineState.Get());
		dx.SetCmdlist()->SetGraphicsRootSignature(Gpipeline.rootSignature.Get());
#pragma endregion
#pragma region �v���~�e�B�u�`��
		// �v���~�e�B�u�`��̐ݒ�R�}���h
		dx.SetCmdlist()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion

#pragma region �`��R�}���h
		// �`��R�}���h
		for (int i = 0; i < ObjectNum; i++) {
			if (!input.IsPush(DIK_SPACE))
			{
				object3d[i].Draw(dx.SetCmdlist());
			}
			else
			{
				object3d[i].SecoundDraw(dx.SetCmdlist());
			}
		}
		/*for (int i = 0; i < LineNum; i++) {
			lineObject[i].Draw(dx.cmdList);
		}*/
#pragma endregion
		// �`��R�}���h�����܂�
#pragma endregion
#pragma region ���\�[�X�o���A��߂�
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		dx.SetCmdlist()->ResourceBarrier(1, &barrierDesc);
#pragma endregion
#pragma region �R�}���h�̃t���b�V��
		dx.CmdFlash();
#pragma endregion

#pragma region �R�}���h�����҂�
		dx.CmdClear();
#pragma endregion
	}

	return 0;
}