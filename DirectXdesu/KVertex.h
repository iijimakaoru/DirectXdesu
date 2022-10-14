#pragma once
#include <DirectXMath.h>
#include "KDirectXCommon.h"
#include "Vector3.h"
#include "Vector2.h"
#include <vector>

using namespace DirectX;

//std::vector<int>;

// ���_�f�[�^�\����
struct Vertex {
	XMFLOAT3 pos;	 // xyz���W
	XMFLOAT3 normal; // �@���x�N�g��
	XMFLOAT2 uv;	 // uv���W
};

class KVertex
{
public:
	KVertex();
	void CreateKVertex(ID3D12Device* dev, std::vector<Vertex>& vertices, std::vector<short>& indices);
	KVertex(ID3D12Device* dev, std::vector<Vertex>& vertices, std::vector<short>& indices);
	void KVertexInit(ID3D12Device* dev, std::vector<Vertex>& vertices, std::vector<short>& indices);
	void VertMap(std::vector<Vertex>& vertices);
	void CreateVBView(std::vector<Vertex>& vertices);

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB;
	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	// ���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff = nullptr;
	// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB;
	// �C���f�b�N�X�o�b�t�@�̐���
	ID3D12Resource* indexBuff = nullptr;
	// �C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	HREFTYPE result;
};

