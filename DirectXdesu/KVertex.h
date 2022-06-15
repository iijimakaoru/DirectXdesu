#pragma once
#include <DirectXMath.h>
#include "KDirectInit.h"
#include "Vector3.h"
#include <vector>
//#include "KModel.h"

using namespace DirectX;

//std::vector<int>;

// ���_�f�[�^�\����
struct Vertex {
	XMFLOAT3 pos;	 // xyz���W
	XMFLOAT3 normal; // �@���x�N�g��
	XMFLOAT2 uv;	 // uv���W
};
// ���_�f�[�^
static std::vector<Vertex> vertices = {
	//  x	  y	    z	 n    u	   v
	// �O
	{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // ����
	{{-5.0f, 5.0f,-5.0f},{},{0.0f,0.0f}}, // ����
	{{ 5.0f,-5.0f,-5.0f},{},{1.0f,1.0f}}, // �E��
	{{ 5.0f, 5.0f,-5.0f},{},{1.0f,0.0f}}, // �E��
	// ��
	{{ 5.0f,-5.0f, 5.0f},{},{1.0f,1.0f}}, // �E��
	{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}, // �E��
	{{-5.0f,-5.0f, 5.0f},{},{0.0f,1.0f}}, // ����
	{{-5.0f, 5.0f, 5.0f},{},{0.0f,0.0f}}, // ����
	// ��
	{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // ����
	{{-5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}}, // ����
	{{-5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}}, // �E��
	{{-5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}, // �E��
	// �E
	{{ 5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}}, // �E��
	{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}, // �E��
	{{ 5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // ����
	{{ 5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}}, // ����
	// ��
	{{ 5.0f,-5.0f,-5.0f},{},{1.0f,1.0f}}, // �E��
	{{ 5.0f,-5.0f, 5.0f},{},{1.0f,0.0f}}, // �E��
	{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}}, // ����
	{{-5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}}, // ����
	// ��
	{{-5.0f, 5.0f,-5.0f},{},{0.0f,1.0f}}, // ����
	{{-5.0f, 5.0f, 5.0f},{},{0.0f,0.0f}}, // ����
	{{ 5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}}, // �E��
	{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}}  // �E��
};

// �C���f�b�N�X�f�[�^
static std::vector<short> indices = {
	// �O
	 0, 1, 2, // �O�p�`1��
	 2, 1, 3, // �O�p�`2��
	// ��
	 4, 5, 6,
	 6, 5, 7,
	 // ��
	  8, 9,10,
	 10, 9,11,
	 // �E
	 12,13,14,
	 14,13,15,
	 // ��
	 16,17,18,
	 18,17,19,
	 // ��
	 20,21,22,
	 22,21,23
};

class KVertex
{
public:
	KVertex(KDirectInit dx);
	void KVertexInit(KDirectInit dx);
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
};

