#pragma once
#include "KVertex.h"
#include <DirectXTex.h>
#include <string>
#include <wrl.h>
#include <map>
#include <mutex>

#include "Vector4.h"

using namespace Microsoft::WRL;

using namespace DirectX;

using namespace std;

class KTexture
{
public:
	KTexture() {};
	void CreateTexture(const std::string& directoryPath, const std::string& filename);

	TexMetadata metadata{};

	ScratchImage scraychImg{};

	ScratchImage mipChain{};

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};

	// テクスチャバッファの生成
	ComPtr<ID3D12Resource> texBuff;

	// SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	// 設定を元にSRV用デスクリプタヒープを生成
	ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;

	// SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

	UINT incrementSize;

	HRESULT result;
};

class KTextureManager
{
public:
	//TextureManagerを取得する
	static KTextureManager* GetInstance() 
	{
		if (textureManager == nullptr)
		{
			textureManager = new KTextureManager();
		}

		return textureManager;
	}

	// 解放
	void Destory()
	{
		delete textureManager;
	}

private:
	KTextureManager() {
		Init();
	};
	~KTextureManager() = default;
	KTextureManager(const KTextureManager & a) {};
	KTextureManager& operator=(const KTextureManager&) { return *this; }

	static KTextureManager* textureManager;

	void Init();

public:
	int textureNum;
};