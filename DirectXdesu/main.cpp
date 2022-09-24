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
	// デバッグレイヤーをオンに
	ID3D12Debug1* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(true);
	}
#endif

	HRESULT result;

#pragma region ウィンドウ
	KWindow win;
#pragma endregion

#pragma region DirectX初期化
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

#pragma region 描画初期化
#pragma region 深度バッファ
	KDepth depth(dx.SetDev().Get(), win);
#pragma endregion
	// 速さ
	float speed = 1.0f;
#pragma region モデル
	KModel triangle = Triangle();
	triangle.CreateModel(dx.SetDev().Get());
	KModel cube = Cube();
	cube.CreateModel(dx.SetDev().Get());
	KModel line = Line();
	line.CreateModel(dx.SetDev().Get());
#pragma endregion

#pragma region テクスチャ初期化
	const wchar_t* msg = L"Resources/mario.jpg";
	const wchar_t* msg2 = L"Resources/いーじゃん.jpg";
	const wchar_t* msg3 = L"Resources/haikei.jpg";
	const wchar_t* msg4 = L"Resources/kitanai.jpg";
	KTexture texture(dx.SetDev().Get(), msg, msg3);
	KTexture texture2(dx.SetDev().Get(), msg2, msg4);
#pragma endregion

#pragma region グラフィックスパイプライン設定
	KGPlin Gpipeline(dx.SetDev().Get());
#pragma region 3Dオブジェクト初期化
	const int ObjectNum = 2;
	const int LineNum = 6;
	// 3Dオブジェクト
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

#pragma region ビュー
	// ビュープロジェクション
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

	// ウィンドウ表示
	// ゲームループ
	while (true)
	{
#pragma region ウィンドウメッセージ
		if (win.breakFlag || input.IsPush(DIK_ESCAPE)) {
			break;
		}
#pragma endregion

		// 更新
#pragma region DirectX毎フレーム処理
#pragma region ウィンドウアップデート
		win.Update();
#pragma endregion

#pragma region inputアップデート
		input.Update();
#pragma endregion

#pragma region キーボード処理
		// 背景色変え
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
		// 画像色変え
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
		// 図形縦回転
		if (input.IsPush(DIK_C) ||
			input.IsPush(DIK_V)) {
			if (input.IsPush(DIK_C)) {
				object3d[0].transform.rot.z += 0.1f;
			}

			if (input.IsPush(DIK_V)) {
				object3d[0].transform.rot.z -= 0.1f;
			}
		}
		////カメラ移動
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
			// angleラジアンy軸回転
			viewProjection->eye.x = viewProjection->lenZ * sinf(viewProjection->angleX);
			viewProjection->eye.y = viewProjection->lenZ * sinf(viewProjection->angleY);
			viewProjection->eye.z = viewProjection->lenZ * cosf(viewProjection->angleX) * cosf(viewProjection->angleY);
		}
		// 横回転
		if (input.IsPush(DIK_RIGHT) ||
			input.IsPush(DIK_LEFT)) {
			if (input.IsPush(DIK_RIGHT)) {
				object3d[0].transform.rot.y -= 0.1f;
			}
			if (input.IsPush(DIK_LEFT)) {
				object3d[0].transform.rot.y += 0.1f;
			}
		}
		//// 移動
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
		// 前ベクトル
		object3d[0].rotResult.x = sin(object3d[0].transform.rot.y) * center.z;
		object3d[0].rotResult.z = cos(object3d[0].transform.rot.y) * center.z;
		// 移動
		/*object3d[0]->transform.pos.x += (speed)*object3d[0]->rotResult.x;
		object3d[0]->transform.pos.z += (speed)*object3d[0]->rotResult.z;*/
		object3d[0].transform.pos.x += (input.IsPush(DIK_RIGHT) - input.IsPush(DIK_LEFT)) * speed;
		object3d[0].transform.pos.z += (input.IsPush(DIK_UP) - input.IsPush(DIK_DOWN)) * speed;

		object3d[1].transform.pos.x = object3d[0].transform.pos.x + 15;
		object3d[1].transform.pos.z = object3d[0].transform.pos.z;
		object3d[1].transform.pos.y = object3d[0].transform.pos.y + 10;

		object3d[1].transform.rot = object3d[0].transform.rot;
#pragma endregion

#pragma region 画像色アップデート
		//material->Update();
#pragma endregion

#pragma region ビューのアップデート
		viewProjection->Update(win.window_width, win.window_height);
#pragma endregion

#pragma region 3Dオブジェクトのアップデート
		for (int i = 0; i < ObjectNum; i++) {
			object3d[i].Update(viewProjection->matView, viewProjection->matProjection);
		}
		/*for (int i = 0; i < LineNum; i++) {
			lineObject[i].Update(viewProjection->matView, viewProjection->matProjection);
		}*/
#pragma endregion

#pragma endregion

		// 描画
#pragma region リソースバリア
		// バックバッファの番号を取得
		UINT bbIndex = dx.SetSChain()->GetCurrentBackBufferIndex();
		// 1.リソースバリアで書き込み可能に変更
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = dx.SetBackBuffers()[bbIndex].Get();
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		dx.SetCmdlist()->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region 描画先
		// 2. 描画先の変更
		// レンダーターゲートビューのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx.SetRtvHeap()->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * dx.SetDev()->GetDescriptorHandleIncrementSize(dx.SetRtvHeapDesc().Type);
		// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depth.dsvHeap->GetCPUDescriptorHandleForHeapStart();
		dx.SetCmdlist()->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
#pragma endregion

#pragma region 画面クリア
		// 3. 画面クリア
		FLOAT clearColor[] = { dx.bRed,dx.bGreen,dx.bBule,0.0f };
		dx.SetCmdlist()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		dx.SetCmdlist()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion

#pragma region 描画
		// 描画コマンドここから
#pragma region ビューポート設定コマンド
		// ビューポート設定コマンド
		D3D12_VIEWPORT viewport{};
		viewport.Width = win.window_width;   // 横幅
		viewport.Height = win.window_height; // 縦幅
		viewport.TopLeftX = 0;                 // 左上x
		viewport.TopLeftY = 0;				   // 左上y
		viewport.MinDepth = 0.0f;			   // 最小深度
		viewport.MaxDepth = 1.0f;			   // 最大深度
		// ビューポート設定コマンドをコマンドリストに積む
		dx.SetCmdlist()->RSSetViewports(1, &viewport);
#pragma endregion
#pragma region シザー矩形設定
		// シザー矩形
		D3D12_RECT scissorRect{};
		scissorRect.left = 0;									// 切り抜き座標左
		scissorRect.right = scissorRect.left + win.window_width;	// 切り抜き座標右
		scissorRect.top = 0;									// 切り抜き座標上
		scissorRect.bottom = scissorRect.top + win.window_height;	// 切り抜き座標下
		// シザー矩形設定コマンドをコマンドリストに積む
		dx.SetCmdlist()->RSSetScissorRects(1, &scissorRect);
#pragma endregion
#pragma region パイプラインステート設定
		// パイプラインステートとルートシグネチャの設定コマンド
		dx.SetCmdlist()->SetPipelineState(Gpipeline.pipelineState.Get());
		dx.SetCmdlist()->SetGraphicsRootSignature(Gpipeline.rootSignature.Get());
#pragma endregion
#pragma region プリミティブ形状
		// プリミティブ形状の設定コマンド
		dx.SetCmdlist()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion

#pragma region 描画コマンド
		// 描画コマンド
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
		// 描画コマンドここまで
#pragma endregion
#pragma region リソースバリアを戻す
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		dx.SetCmdlist()->ResourceBarrier(1, &barrierDesc);
#pragma endregion
#pragma region コマンドのフラッシュ
		dx.CmdFlash();
#pragma endregion

#pragma region コマンド完了待ち
		dx.CmdClear();
#pragma endregion
	}

	return 0;
}