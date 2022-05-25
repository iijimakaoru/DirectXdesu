#pragma once
#include "KVertex.h"

// �C���f�b�N�X�f�[�^
static unsigned short indices[] = {
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

class KIndex
{
public:
	KIndex(KDirectInit dx, KVertex vertex);
	void IndexInit(KDirectInit dx, KVertex vertex);

	// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB;
	// �C���f�b�N�X�o�b�t�@�̐���
	ID3D12Resource* indexBuff = nullptr;
	// �C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
};

