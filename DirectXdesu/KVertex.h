#pragma once
#include <DirectXMath.h>
#include "KDirectInit.h"
#include "Vector3.h"

using namespace DirectX;

// ���_�f�[�^�\����
struct Vertex {
	XMFLOAT3 pos;	 // xyz���W
	XMFLOAT3 normal; // �@���x�N�g��
	XMFLOAT2 uv;	 // uv���W
};

// ���_�f�[�^
static XMFLOAT3 vertices[] = {
	{-0.5f,-0.5f,0.0f},
	{-0.5f,+0.5f,0.0f},
	{+0.5f,-0.5f,0.0f}
};

// �C���f�b�N�X�f�[�^
static unsigned short indices[] = {
	// �O
	 0, 1, 2, // �O�p�`1��
};

class KVertex
{
public:
	KVertex(ID3D12Device* dev);
	void KVertexInit(ID3D12Device* dev);
	void VertMap();
	void CreateVBView();

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

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[3] = {
		{// xyz���W
			"POSITION",										// �Z�}���e�B�b�N��
			0,												// �����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X
			DXGI_FORMAT_R32G32B32_FLOAT,					// �v�f���ƃr�b�g����\��
			0,												// ���̓X���b�g�C���f�b�N�X
			D3D12_APPEND_ALIGNED_ELEMENT,					// �f�[�^�̃I�t�Z�b�g
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,		// ���̓f�[�^���
			0												// ��x�ɕ`�悷��C���X�^���X��
		},
		{// �@���x�N�g��
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{// uv���W
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	HRESULT result;
};

