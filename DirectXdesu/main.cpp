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
#include "KObject3D.h"
#ifdef DEBUG
#include <iostream>
#endif
#pragma comment(lib, "d3dcompiler.Lib")

// 定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial {
	XMFLOAT4 color; // 色
};

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

	// 画像の色
	float colorR = 1.0f;
	float colorG = 1.0f;
	float colorB = 1.0f;
	float colorA = 1.0f;

	// カメラの距離
	float lenZ = -100;
#pragma region 頂点データ
	KVertex vertex(dx);
#pragma endregion

#pragma region 頂点シェーダー
	KVertexShader vtShader(dx);
#pragma endregion

#pragma region ピクセルシェーダ
	KPixelShader pxShader(dx);
#pragma endregion

#pragma region グラフィックスパイプライン設定
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	// シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vtShader.vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vtShader.vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = pxShader.psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = pxShader.psBlob->GetBufferSize();
	// サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // 背面をカリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	pipelineDesc.RasterizerState.DepthClipEnable = true;
	// ブレンドステート
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = vertex.inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(vertex.inputLayout);
	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// その他の設定
	pipelineDesc.NumRenderTargets = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	pipelineDesc.SampleDesc.Count = 1;
	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	// 半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
#pragma region 定数バッファ
	// ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 定数バッファの生成
	ID3D12Resource* constBufferMaterial = nullptr;
	dx.result = dx.dev->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBufferMaterial));
	assert(SUCCEEDED(dx.result));
	// 定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	dx.result = constBufferMaterial->Map(
		0,
		nullptr,
		(void**)&constMapMaterial);
	assert(SUCCEEDED(dx.result));

#pragma region 3Dオブジェクト初期化
	KObject3D object3d(dx.result, dx.dev);
#pragma endregion

#pragma region 行列
	// ビュー変換行列
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, lenZ);
	XMFLOAT3 target(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	float angleX = 0.0f; // カメラの回転角X
	float angleY = 0.0f; // カメラの回転角Y

	// 射影変換行列の計算
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),						// 上下画角45度
		(float)win.window_width / win.window_height,	// アスペクト比(画面横幅/画面縦幅)
		0.1f, 1000.0f									// 前端、奥端
	);
#pragma endregion

#pragma endregion
	// 値を書き込むと自動的に転送される
	constMapMaterial->color = XMFLOAT4(colorR, colorG, colorB, colorA);
	// デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	// ルートパラメータの設定
	D3D12_ROOT_PARAMETER rootParam[3] = {};
	// 定数バッファ0番
	rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[0].Descriptor.ShaderRegister = 0;
	rootParam[0].Descriptor.RegisterSpace = 0;
	rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// テクスチャレジスタ0番
	rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
	rootParam[1].DescriptorTable.NumDescriptorRanges = 1;
	rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// 定数バッファ1番
	rootParam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[2].Descriptor.ShaderRegister = 1;
	rootParam[2].Descriptor.RegisterSpace = 0;
	rootParam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// テクスチャサンブラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	// ルートシグネチャ
	ID3D12RootSignature* rootSignature;
	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam;
	rootSignatureDesc.NumParameters = _countof(rootParam);
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	// ルートシグネチャのシリアライズ
	ID3DBlob* rootSigBlob = nullptr;
	dx.result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &pxShader.errorBlob);
	assert(SUCCEEDED(dx.result));
	dx.result = dx.dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(dx.result));
	rootSigBlob->Release();
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature;
	// デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true; // 深度テスト
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // 書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // 小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	// パイプラインステート
	ID3D12PipelineState* pipelineState = nullptr;
	dx.result = dx.dev->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(dx.result));
#pragma endregion

#pragma region テクスチャ初期化
	KTexture texture(dx, vertex);
#pragma endregion

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
		win.Update();
		input.Update(dx.result);
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
			colorR = 1.0f;
			colorG = 0.0f;
			colorB = 1.0f;
		}
		else {
			colorR = 1.0f;
			colorG = 1.0f;
			colorB = 1.0f;
		}
		constMapMaterial->color = XMFLOAT4(colorR, colorG, colorB, colorA);
		//カメラ移動
		if (input.IsPush(DIK_D) || input.IsPush(DIK_A) ||
			input.IsPush(DIK_W) || input.IsPush(DIK_S)) {
			if (input.IsPush(DIK_D)) {
				angleX += XMConvertToRadians(1.0f);
			}
			else if (input.IsPush(DIK_A)) {
				angleX -= XMConvertToRadians(1.0f);
			}

			if (input.IsPush(DIK_W)) {
				angleY -= XMConvertToRadians(1.0f);
			}
			else if (input.IsPush(DIK_S)) {
				angleY += XMConvertToRadians(1.0f);
			}

			// angleラジアンy軸回転
			eye.x = lenZ * sinf(angleX);
			eye.y = lenZ * sinf(angleY);
			eye.z = lenZ * cosf(angleX) * cosf(angleY);
		}
		//// 図形回転
		//if (input.IsPush(DIK_W) ||
		//	input.IsPush(DIK_S) ||
		//	input.IsPush(DIK_A) ||
		//	input.IsPush(DIK_D)) {
		//	if (input.IsPush(DIK_W)) {
		//		rotation.x = 1.0f;
		//	}
		//	else if (input.IsPush(DIK_S)) {
		//		rotation.x = -1.0f;
		//	}
		//	else {
		//		rotation.x = 0.0f;
		//	}

		//	if (input.IsPush(DIK_A)) {
		//		rotation.y = -1.0f;
		//	}
		//	else if (input.IsPush(DIK_D)) {
		//		rotation.y = 1.0f;
		//	}
		//	else {
		//		rotation.y = 0.0f;
		//	}
		//}
		//else {
		//	rotation.x = 0.0f;
		//	rotation.y = 0.0f;
		//	rotation.z = 0.0f;
		//}
		// 移動
		if (input.IsPush(DIK_UP) ||
			input.IsPush(DIK_DOWN) ||
			input.IsPush(DIK_RIGHT) ||
			input.IsPush(DIK_LEFT)) {
			speed = 1.0f;
			if (input.IsPush(DIK_UP)) {
				object3d.object3d[0].pos.y += speed;
			}
			if (input.IsPush(DIK_DOWN)) {
				object3d.object3d[0].pos.y -= speed;
			}
			if (input.IsPush(DIK_RIGHT)) {
				object3d.object3d[0].pos.x += speed;
			}
			if (input.IsPush(DIK_LEFT)) {
				object3d.object3d[0].pos.x -= speed;
			}
		}
		else {
			speed = 0.0f;
		}
#pragma endregion

#pragma region 行列
		// ビュー行列の計算
		matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
		// 射影変換行列の計算
		matProjection = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),						// 上下画角45度
			(float)win.window_width / win.window_height,	// アスペクト比(画面横幅/画面縦幅)
			0.1f, 1000.0f									// 前端、奥端
		);
#pragma endregion

#pragma region 3Dオブジェクトのアップデート
		object3d.Update(matView, matProjection);
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
		dx.cmdList->SetPipelineState(pipelineState);
		dx.cmdList->SetGraphicsRootSignature(rootSignature);
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
		dx.cmdList->SetGraphicsRootConstantBufferView(0, constBufferMaterial->GetGPUVirtualAddress());
		// SRV
		dx.cmdList->SetDescriptorHeaps(1, &texture.srvHeap);
		// 先頭ハンドルを取得
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = texture.srvHeap->GetGPUDescriptorHandleForHeapStart();
		// SRVヒープの先頭にあるSRVをルートパラメータ1番の設定
		dx.cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
#pragma region 描画コマンド
		// 描画コマンド
		object3d.Draw(dx.cmdList, vertex.vbView, vertex.ibView, _countof(indices));
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
		if (dx.fence->GetCompletedValue() != dx.fenceVal)
		{
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