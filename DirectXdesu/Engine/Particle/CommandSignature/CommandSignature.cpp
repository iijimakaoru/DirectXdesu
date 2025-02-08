#include "CommandSignature.h"
#include "KDirectXCommon.h"

void CommandSignature::Create()
{
	ID3D12Device* device = KDirectXCommon::GetInstance()->GetDevice();

	// パーティクルコマンドシグネチャ
	D3D12_INDIRECT_ARGUMENT_DESC Args[1];
	Args[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;

	D3D12_COMMAND_SIGNATURE_DESC particleCommandSingatureDescription = {};
	particleCommandSingatureDescription.ByteStride = 36;
	particleCommandSingatureDescription.NumArgumentDescs = 1;
	particleCommandSingatureDescription.pArgumentDescs = Args;

	device->CreateCommandSignature(
		&particleCommandSingatureDescription,
		NULL,
		IID_PPV_ARGS(commandSignature.GetAddressOf()));
}

ID3D12CommandSignature* CommandSignature::GetCommandSignature()
{
	return commandSignature.Get();
}
