#include "FbxObject3D.h"
#include "FbxLoader.h"
#include "MyMath.h"

KGPlin* FbxObject3D::pipeline = nullptr;

void FbxObject3D::Init()
{
	// ヒープ設定
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	// 定数バッファB1
	CD3DX12_RESOURCE_DESC buff = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&buff,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));

	CD3DX12_RESOURCE_DESC buffSkin = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff);

	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&buffSkin,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin));
	assert(SUCCEEDED(result));

	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

	// 定数バッファデータ転送
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < MAX_BONES; i++)
	{
		constMapSkin->bones[i] = MyMathUtility::MakeIdentity();
	}
	constBuffSkin->Unmap(0, nullptr);
}

void FbxObject3D::Update(ViewProjection* viewProjection)
{
	// マトリックス
	KMyMath::Matrix4 matScale, matRot, matTrans;

	// 親オブジェクト要素
	matScale = MyMathUtility::MakeIdentity();
	matScale = MyMathUtility::MakeScaling(scale);
	matRot = MyMathUtility::MakeIdentity();
	matRot = MyMathUtility::MakeRotation({ DirectX::XMConvertToRadians(rotation.x),
		DirectX::XMConvertToRadians(rotation.y),DirectX::XMConvertToRadians(rotation.z) });
	matTrans = MyMathUtility::MakeIdentity();
	matTrans = MyMathUtility::MakeTranslation(position);

	matWorld = MyMathUtility::MakeIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	const KMyMath::Matrix4& matViewProjection = viewProjection->GetMatView() * viewProjection->GetMatPro();

	const KMyMath::Matrix4& modelTransform = model->GetModelTransform();

	const KMyMath::Vector3& cameraPos = viewProjection->GetEye();

	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	constMap->viewproj = matViewProjection;
	constMap->world = modelTransform * matWorld;
	constMap->cameraPos = cameraPos;
	constBuffTransform->Unmap(0, nullptr);

	// ボーン配列
	std::vector<FbxModel::Bone>& bones = model->GetBones();

	// アニメーション
	if (isPlay)
	{
		// 1フレーム進める
		currentTime += frameTime;
		if (currentTime > endTime)
		{
			currentTime = startTime;
		}
	}

	// 定数バッファデータ転送
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (size_t i = 0; i < bones.size(); i++)
	{
		// 今の姿勢行列
		KMyMath::Matrix4 matCurrentPose;
		// 今の姿勢行列を取得
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		// XMMATRIXに変換
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		// 合成してスキニング行列に
		constMapSkin->bones[i] = bones[i].invInitialPose * matCurrentPose;
	}
	constBuffSkin->Unmap(0, nullptr);
}

void FbxObject3D::Draw()
{
	ID3D12GraphicsCommandList* cmdList = KDirectXCommon::GetInstance()->GetCmdlist();
	if (model == nullptr)
	{
		return;
	}

	pipeline->Setting();
	pipeline->Update(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());

	model->Draw();
}

void FbxObject3D::PlayAnimation()
{
	// Fbxシーン取得
	FbxScene* fbxScene = model->GetFbxScene();
	// 0番のアニメーション取得
	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);

	fbxScene->SetCurrentAnimationStack(animstack);
	// アニメーションの名前取得
	const char* animStackName = animstack->GetName();
	// アニメーションの時間取得
	FbxTakeInfo* takeInfo = fbxScene->GetTakeInfo(animStackName);
	// 開始時間取得
	startTime = takeInfo->mLocalTimeSpan.GetStart();
	// 終了時間取得
	endTime = takeInfo->mLocalTimeSpan.GetStop();
	// 開始時間に合わせる
	currentTime = startTime;
	// 再生中状態にする
	isPlay = true;
}
