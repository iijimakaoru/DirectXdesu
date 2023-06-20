#include "FbxObject3D.h"
#include "FbxLoader.h"

KGPlin* FbxObject3D::pipeline = nullptr;

using namespace DirectX;

void FbxObject3D::Init()
{
	// �q�[�v�ݒ�
	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	// �萔�o�b�t�@B1
	CD3DX12_RESOURCE_DESC buff = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);

	// �萔�o�b�t�@�̐���
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

	// �萔�o�b�t�@�f�[�^�]��
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < MAX_BONES; i++)
	{
		constMapSkin->bones[i] = XMMatrixIdentity();
	}
	constBuffSkin->Unmap(0, nullptr);
}

void FbxObject3D::Update(ViewProjection& viewProjection)
{
	// �}�g���b�N�X
	XMMATRIX matScale, matRot, matTrans;

	// �e�I�u�W�F�N�g�v�f
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation.z);
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	const XMMATRIX& matViewProjection = viewProjection.matView * viewProjection.matProjection;

	const XMMATRIX& modelTransform = model->GetModelTransform();

	const XMFLOAT3& cameraPos = viewProjection.eye;

	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	constMap->viewproj = matViewProjection;
	constMap->world = modelTransform * matWorld;
	constMap->cameraPos = cameraPos;
	constBuffTransform->Unmap(0, nullptr);

	// �{�[���z��
	std::vector<FbxModel::Bone>& bones = model->GetBones();

	// �A�j���[�V����
	if (isPlay)
	{
		// 1�t���[���i�߂�
		currentTime += frameTime;
		if (currentTime > endTime)
		{
			currentTime = startTime;
		}
	}

	// �萔�o�b�t�@�f�[�^�]��
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++)
	{
		// ���̎p���s��
		XMMATRIX matCurrentPose;
		// ���̎p���s����擾
		FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
		// XMMATRIX�ɕϊ�
		FbxLoader::ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);
		// �������ăX�L�j���O�s���
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

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffSkin->GetGPUVirtualAddress());

	model->Draw();
}

void FbxObject3D::PlayAnimation()
{
	// Fbx�V�[���擾
	FbxScene* fbxScene = model->GetFbxScene();
	// 0�Ԃ̃A�j���[�V�����擾
	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);

	fbxScene->SetCurrentAnimationStack(animstack);
	// �A�j���[�V�����̖��O�擾
	const char* animStackName = animstack->GetName();
	// �A�j���[�V�����̎��Ԏ擾
	FbxTakeInfo* takeInfo = fbxScene->GetTakeInfo(animStackName);
	// �J�n���Ԏ擾
	startTime = takeInfo->mLocalTimeSpan.GetStart();
	// �I�����Ԏ擾
	endTime = takeInfo->mLocalTimeSpan.GetStop();
	// �J�n���Ԃɍ��킹��
	currentTime = startTime;
	// �Đ�����Ԃɂ���
	isPlay = true;
}