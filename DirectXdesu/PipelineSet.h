#pragma once
struct PipelineSet
{
	// �p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> pipelineState;
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;
};