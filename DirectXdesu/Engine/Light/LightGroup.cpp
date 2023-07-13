#include "LightGroup.h"
#include "KDirectXCommon.h"

void LightGroup::Init()
{
	HRESULT result;

	DefaultLightSetting();

	// ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 定数バッファの生成
	constBuff = nullptr;
	result = KDirectXCommon::GetInstance()->GetDev()->CreateCommittedResource(
		&cbHeapProp, // 
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc, // 
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	TransferConstBuffer();
}

void LightGroup::Update()
{
	if (dirty)
	{
		TransferConstBuffer();
		dirty = false;
	}
}

void LightGroup::Draw(UINT rootParameterIndex)
{
	KDirectXCommon::GetInstance()->GetCmdlist()->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff->GetGPUVirtualAddress());
}

LightGroup* LightGroup::Create()
{
	LightGroup* instance = new LightGroup();
	instance->Init();
	return instance;
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;
	// 定数バッファデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->ambientColor = ambientColor;

		for (int i = 0; i < DirLightNum; i++)
		{
			if (dirLights[i].IsActive())
			{
				constMap->dirLights[i].active = 1;
				constMap->dirLights[i].lightv = -dirLights[i].GetLightDir();
				constMap->dirLights[i].lightcolor = dirLights[i].GetLightColor();
			}
			else
			{
				constMap->dirLights[i].active = 0;
			}
		}
		constBuff->Unmap(0, nullptr);
	}
}

void LightGroup::SetAmbientColor(const XMFLOAT3& color)
{
	ambientColor = color;
	dirty = true;
}

void LightGroup::SetDirLightActive(int index, bool isActive)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetActive(isActive);
}

void LightGroup::SetDirLightDir(int index, const XMVECTOR& lightdir)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetLightDir(lightdir);
	dirty = true;
}

void LightGroup::SetDirLightColor(int index, const XMFLOAT3& lightcolor)
{
	assert(0 <= index && index < DirLightNum);
	dirLights[index].SetLightColor(lightcolor);
	dirty = true;
}

void LightGroup::DefaultLightSetting()
{
	dirLights[0].SetActive(true);
	dirLights[0].SetLightColor({ 1,1,1 });
	dirLights[0].SetLightDir({ 0.0f,-1.0f,0.0f,0 });

	dirLights[1].SetActive(true);
	dirLights[1].SetLightColor({ 0.5f,1,0.5f });
	dirLights[1].SetLightDir({ 0.5f,0.1f,0.2f,0 });

	dirLights[2].SetActive(true);
	dirLights[2].SetLightColor({ 1,0.5f,0.5f });
	dirLights[2].SetLightDir({ -0.5f,0.1f,-0.2f,0 });
}