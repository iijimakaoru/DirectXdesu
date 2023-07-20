#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <d3dx12.h>
#include "KGPlin.h"
#include <wrl.h>
#include "KTexture.h"

#include "MyMath.h"

#include "TextureManager.h"

class Sprite
{
public:
	static void StaticInit();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="spriteCommon"></param>
	void Init();

	// �X�V
	void Update(KMyMath::Vector2 pos, KMyMath::Vector2 scale, float rot, KMyMath::Vector4 color);

	/// <summary>
	/// �`����
	/// </summary>
	/// <param name="texture"></param>
	void DrawCommand(TextureData texData);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="texture"></param>
	/// <param name="pos"></param>
	/// <param name="setSize_"></param>
	/// <param name="rot"></param>
	/// <param name="color"></param>
	/// <param name="isFlipX_"></param>
	/// <param name="isFlipY_"></param>
	/// <param name="anchorPoint_"></param>
	void Draw(TextureData& texData, KMyMath::Vector2 pos = { 0.0f,0.0f }, KMyMath::Vector2 setSize_ = { 100.0f,100.0f }, float rot = 0.0f,
		KMyMath::Vector4 color = {1.0f,1.0f,1.0f,1.0f}, bool isFlipX_ = false, bool isFlipY_ = false, KMyMath::Vector2 anchorPoint_ = { 0.5f,0.5f });

	/// <summary>
	/// �p�C�v���C���Z�b�^�[
	/// </summary>
	/// <param name="pipeline_"></param>
	void SetPipeline(KGPlin* pipeline_);

	// �|�W�V�����Q�b�^�[
	const KMyMath::Vector2 GetPos() const;

private:
	// �萔�o�b�t�@�}�e���A��
	void CreateCBMaterial();

	// ���_�A�C���f�b�N�X����
	void CreateVertexIndex();

	// �萔�o�b�t�@�g�����X�t�H�[��
	void CreateCBTransform();

private:
	struct Vertex
	{
		KMyMath::Vector3 pos;
		KMyMath::Vector2 uv;
	};

	enum VertexNumber
	{
		LB, // ����
		LT, // ����
		RB, // �E��
		RT, // �E��
	};

	HRESULT result;

	// ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;

	// ���_�}�b�v
	Vertex* vertMap = nullptr;

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// �C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;

	// �C���f�b�N�X�}�b�v
	uint16_t* indexMap = nullptr;

	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

	// �萔�o�b�t�@�̃}�b�v
	KMyMath::Vector4* constMapMaterial = nullptr;

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;

	// �萔�o�b�t�@�̃}�b�v
	KMyMath::Matrix4* constMapTransform = nullptr;

	// �ʒu
	KMyMath::Vector2 position = { 0.0f,0.0f };
	// ��]
	float rotation = 0.0f;
	// �F
	KMyMath::Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	// �\���T�C�Y
	KMyMath::Vector2 size_ = { 100.0f,100.0f };
	// �A���J�[�|�C���g
	KMyMath::Vector2 anchorPoint = { 0.5f,0.5f };
	// ���E�t���b�v
	int flipX = 1;
	// �㉺�t���b�v
	int flipY = 1;
	// ��\��
	bool isInvisible = false;

	// �f�o�C�X
	static Microsoft::WRL::ComPtr<ID3D12Device> device;

	// �R�}���h���X�g
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList;

	static KGPlin* pipeline;

	static KMyMath::Matrix4 matPro;
};