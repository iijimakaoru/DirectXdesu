#include "KVertex.h"
#include "MyMath.h"

KVertex::KVertex(ID3D12Device* dev, std::vector<VertexPosNormalUV>& vertices, std::vector<unsigned short>& indices) 
{
	KVertexInit(dev, vertices, indices);
}

void KVertex::KVertexInit(ID3D12Device* dev, std::vector<VertexPosNormalUV>& vertices, std::vector<unsigned short>& indices) 
{
#pragma region ���_
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUV) * vertices.size());

	CD3DX12_HEAP_PROPERTIES heap1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	CD3DX12_RESOURCE_DESC vB = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
	result = dev->CreateCommittedResource(
		&heap1,
		D3D12_HEAP_FLAG_NONE,
		&vB,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z���������擾
	VertexPosNormalUV* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	std::copy(vertices.begin(), vertices.end(), vertMap);
	// �q���������
	vertBuff->Unmap(0, nullptr);

	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_����̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);
#pragma endregion

#pragma region �C���f�b�N�X
	// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	CD3DX12_RESOURCE_DESC iB = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);
	result = dev->CreateCommittedResource(
		&heap1,
		D3D12_HEAP_FLAG_NONE,
		&iB,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	// �C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	// �S�C���f�b�N�X�ɑ΂���
	std::copy(indices.begin(), indices.end(), indexMap);
	// �}�b�s���O����
	indexBuff->Unmap(0, nullptr);

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
#pragma endregion

#pragma region �@���̌v�Z
	for (int i = 0; i < indices.size() / 3; i++) 
	{
		// �O�p�`�P���ƂɌv�Z
		unsigned short indices0 = indices[i * 3 + 0];
		unsigned short indices1 = indices[i * 3 + 1];
		unsigned short indices2 = indices[i * 3 + 2];
		// �O�p�`���\�����钸�_���W���x�N�g���ɑ��
		DirectX::XMFLOAT3 pV0 = MyMathConvert::ChangeVector3toXMfloat3(vertices[indices0].pos);
		DirectX::XMVECTOR p0 = XMLoadFloat3(&pV0);
		DirectX::XMFLOAT3 pV1 = MyMathConvert::ChangeVector3toXMfloat3(vertices[indices1].pos);
		DirectX::XMVECTOR p1 = XMLoadFloat3(&pV1);
		DirectX::XMFLOAT3 pV2 = MyMathConvert::ChangeVector3toXMfloat3(vertices[indices2].pos);
		DirectX::XMVECTOR p2 = XMLoadFloat3(&pV2);
		// p0 �� p1�x�N�g���Ap0 �� p2�x�N�g�����v�Z (�x�N�g���̌��Z)
		DirectX::XMVECTOR v1 = DirectX::XMVectorSubtract(p1, p0);
		DirectX::XMVECTOR v2 = DirectX::XMVectorSubtract(p2, p0);
		// �O�ς͗������琂���ȃx�N�g��
		DirectX::XMVECTOR normal = DirectX::XMVector3Cross(v1, v2);
		// ���K��(�������P�ɂ���)
		normal = DirectX::XMVector3Normalize(normal);
		// ���߂��@���𒸓_�f�[�^�ɑ��
		DirectX::XMFLOAT3 nV0 = MyMathConvert::ChangeVector3toXMfloat3(vertices[indices0].normal);
		DirectX::XMStoreFloat3(&nV0, normal);
		DirectX::XMFLOAT3 nV1 = MyMathConvert::ChangeVector3toXMfloat3(vertices[indices1].normal);
		DirectX::XMStoreFloat3(&nV1, normal);
		DirectX::XMFLOAT3 nV2 = MyMathConvert::ChangeVector3toXMfloat3(vertices[indices2].normal);
		DirectX::XMStoreFloat3(&nV2, normal);
	}
#pragma endregion
}