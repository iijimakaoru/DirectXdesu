#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <wrl.h>
#include <vector>

#include "KUtility.h"
#include "KPipelineState.h"
#include "KRootSignature.h"
#include "KShader.h"

using namespace Microsoft::WRL;

using namespace DirectX;

// �萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
struct ConstBufferDataMaterial {
	XMFLOAT4 color; // �F
};

class KMaterial
{
public:
	KMaterial() = default;
	~KMaterial();
	void Initialize();

private:
	KMaterial(const KMaterial&) = delete;
	KMaterial& operator=(const KMaterial&) = delete;

public:
	// �e�N�X�`���f�[�^
	KTextureData* textureData;

	// ���_���C�A�E�g
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayouts;

	// �u�����h�X�e�[�g
	D3D12_BLEND_DESC blenddesc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	// �p�C�v���C���X�e�[�g
	std::unique_ptr<KPipelineState> pipelineState;

	// ���[�g�V�O�l�`��
	std::unique_ptr<KRootSignature> rootSignature;

	// �V�F�[�_�[
	std::unique_ptr<KShader> shader;

	//�[�x�t���O
	bool depthFlag = true;
	char PADING[3]{};

	D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK;
	char PADING2[4]{};
};

