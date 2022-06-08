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
	// デバッグレイヤーをオンに
	ID3D12Ddebug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif

#pragma region ウィンドウ
	KWindow win;
#pragma endregion

#pragma region DirectX初期化
	KDirectInit dx(win);
	KInput input(dx.result, win.window, win.handle);
#pragma endregion

#pragma region 描画初期化
#pragma region 深度バッファ
	KDepth depth(dx, win);
#pragma endregion
	// 速さ
	float speed = 0.0f;
	float speedY = 0.0f;

#pragma region 頂点データ
	KVertex vertex(dx);
#pragma endregion

#pragma region グラフィックスパイプライン設定
	KGPlin Gpipeline(dx, dx.result, dx.dev, win.window_width, win.window_height, vertex);
#pragma endregion

#pragma region テクスチャ初期化
	KTexture texture(dx, vertex);
#pragma endregion
	Vector3 center = { 0,0,1 };

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
		input.Update(dx.result);
#pragma endregion

#pragma region キーボード処理
		// 背景色変え
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
		// 画像色変え
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
		// 図形縦回転
		if (input.IsPush(DIK_F) ||
			input.IsPush(DIK_V)) {
			if (input.IsPush(DIK_F)) {
				Gpipeline.object3d->object3d[0].rot.x += 0.1f;
			}

			if (input.IsPush(DIK_V)) {
				Gpipeline.object3d->object3d[0].rot.x -= 0.1f;
			}
		}
		////カメラ移動
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

			// angleラジアンy軸回転
			Gpipeline.viewProjection->eye.x = Gpipeline.viewProjection->lenZ * sinf(Gpipeline.viewProjection->angleX);
			Gpipeline.viewProjection->eye.y = Gpipeline.viewProjection->lenZ * sinf(Gpipeline.viewProjection->angleY);
			Gpipeline.viewProjection->eye.z = Gpipeline.viewProjection->lenZ * cosf(Gpipeline.viewProjection->angleX) * cosf(Gpipeline.viewProjection->angleY);
		}
		// 移動
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

		// 前ベクトル
		Gpipeline.object3d->rotResult[0].x = sin(Gpipeline.object3d->object3d[0].rot.y) * center.z;
		Gpipeline.object3d->rotResult[0].z = cos(Gpipeline.object3d->object3d[0].rot.y) * center.z;
		// 移動
		Gpipeline.object3d->object3d[0].pos.x += (speedY);
		Gpipeline.object3d->object3d[0].pos.y += (speed);
#pragma region 画像色アップデート
		Gpipeline.material->Update();
#pragma endregion

#pragma region ビューのアップデート
		Gpipeline.viewProjection->Update(win.window_width, win.window_height);
#pragma endregion

#pragma region 3Dオブジェクトのアップデート
		Gpipeline.object3d->Update(Gpipeline.viewProjection->matView, Gpipeline.viewProjection->matProjection);
#pragma endregion

#pragma endregion

		// 描画
#pragma region リソースバリア
		// バックバッファの番号を取得
		UINT bbIndex = dx.swapChain->GetCurrentBackBufferIndex();
		// 1.リソースバリアで書き込み可能に変更
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = dx.backBuffers[bbIndex];
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		dx.cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region 描画先
		// 2. 描画先の変更
		// レンダーターゲートビューのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx.rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * dx.dev->GetDescriptorHandleIncrementSize(dx.rtvHeapDesc.Type);
		// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depth.dsvHeap->GetCPUDescriptorHandleForHeapStart();
		dx.cmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
#pragma endregion

#pragma region 画面クリア
		// 3. 画面クリア
		FLOAT clearColor[] = { dx.bRed,dx.bGreen,dx.bBule,0.0f };
		dx.cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		dx.cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
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
		dx.cmdList->RSSetViewports(1, &viewport);
#pragma endregion
#pragma region シザー矩形設定
		// シザー矩形
		D3D12_RECT scissorRect{};
		scissorRect.left = 0;									// 切り抜き座標左
		scissorRect.right = scissorRect.left + win.window_width;	// 切り抜き座標右
		scissorRect.top = 0;									// 切り抜き座標上
		scissorRect.bottom = scissorRect.top + win.window_height;	// 切り抜き座標下
		// シザー矩形設定コマンドをコマンドリストに積む
		dx.cmdList->RSSetScissorRects(1, &scissorRect);
#pragma endregion
#pragma region パイプラインステート設定
		// パイプラインステートとルートシグネチャの設定コマンド
		dx.cmdList->SetPipelineState(Gpipeline.pipelineState);
		dx.cmdList->SetGraphicsRootSignature(Gpipeline.rootSignature);
#pragma endregion
#pragma region プリミティブ形状
		// プリミティブ形状の設定コマンド
		dx.cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#pragma endregion
		// インデックスバッファビューの設定コマンド
		dx.cmdList->IASetIndexBuffer(&vertex.ibView);
#pragma region 頂点バッファビューの設定コマンド
		// 頂点バッファビューの設定コマンド
		dx.cmdList->IASetVertexBuffers(0, 1, &vertex.vbView);
#pragma endregion
		// CBV
		dx.cmdList->SetGraphicsRootConstantBufferView(0, Gpipeline.material->constBufferMaterial->GetGPUVirtualAddress());
		// SRV
		dx.cmdList->SetDescriptorHeaps(1, &texture.srvHeap);
		// 先頭ハンドルを取得
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = texture.srvHeap->GetGPUDescriptorHandleForHeapStart();
		// SRVヒープの先頭にあるSRVをルートパラメータ1番の設定
		dx.cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
#pragma region 描画コマンド
		// 描画コマンド
		Gpipeline.object3d->Draw(dx.cmdList, vertex.vbView, vertex.ibView, _countof(indices));
#pragma endregion
		// 描画コマンドここまで
#pragma endregion
#pragma region リソースバリアを戻す
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		dx.cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion
#pragma region コマンドのフラッシュ
		// 命令のクローズ
		dx.result = dx.cmdList->Close();
		assert(SUCCEEDED(dx.result));
		// コマンドリストの実行
		ID3D12CommandList* cmdLists[] = { dx.cmdList };
		dx.cmdQueue->ExecuteCommandLists(1, cmdLists);
		// 画面に表示するバッファをフリップ(裏表の入れ替え)
		dx.result = dx.swapChain->Present(1, 0);
		assert(SUCCEEDED(dx.result));
#pragma endregion

#pragma region コマンド完了待ち
		// コマンドの完了を待つ
		dx.cmdQueue->Signal(dx.fence, ++dx.fenceVal);
		if (dx.fence->GetCompletedValue() != dx.fenceVal) {
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			dx.fence->SetEventOnCompletion(dx.fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
		// キューをクリア
		dx.result = dx.cmdAllocater->Reset();
		assert(SUCCEEDED(dx.result));
		// 再びコマンドを貯める準備
		dx.result = dx.cmdList->Reset(dx.cmdAllocater, nullptr);
		assert(SUCCEEDED(dx.result));
#pragma endregion
	}

	return 0;
}