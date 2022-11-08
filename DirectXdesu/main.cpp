#include "KWinApp.h"
#include "KDirectXCommon.h"
#include "KInput.h"
#include "KSpriteCommon.h"
#include "Sprite.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	HRESULT result;
#pragma region ��Տ�����
#pragma region �E�B���h�E
	KWinApp* win = nullptr;
	win = new KWinApp();
#pragma endregion
#pragma region DirectX������
	// KDirectCommon
	KDirectXCommon* dxCommon = nullptr;
	dxCommon = new KDirectXCommon();
	dxCommon->Init(win);
	// �L�[�{�[�h����
	KInput* input = nullptr;
	input = new KInput();
	input->Init(win);
#pragma endregion
	SpriteCommon* spriteCommon = nullptr;

	spriteCommon = new SpriteCommon();
	spriteCommon->Init(dxCommon);

	spriteCommon->SpriteCommonLoadTexture(0, L"Resources/haikei.jpg");
#pragma endregion

#pragma region �V�[���̏�����
	Sprite* sprite = new Sprite();
	sprite->Init(spriteCommon);

	sprite->SpriteCreate();
	//sprite->SpriteTransferVertexBuffer();
#pragma endregion

	// �E�B���h�E�\��
	// �Q�[�����[�v
	while (true)
	{
		// �X�V
#pragma region ��Ղ̍X�V
		// �E�B���h�E���b�Z�[�W
		if (win->ProcessMessage() || input->IsPush(DIK_ESCAPE))
		{
			break;
		}

		// input�X�V
		input->Update();

		//sprite->SpriteUpdate();
#pragma endregion

#pragma region �`��
		// �`��J�n
		dxCommon->PreDraw();

		sprite->SpriteDraw();
		
		// �`��I��
		dxCommon->PostDraw();
#pragma endregion
	}

#pragma region �V�[���̏I��
	// 
	delete sprite;
#pragma endregion

#pragma region ��Ղ̏I��
	// 
	delete spriteCommon;
	// ���͉��
	delete input;
	// DirectXCommon���
	delete dxCommon;
	// WindowsAPI�I������
	win->Finalize();
	// WindowsAPI���
	delete win;
	win = nullptr;
#pragma endregion

	return 0;
}