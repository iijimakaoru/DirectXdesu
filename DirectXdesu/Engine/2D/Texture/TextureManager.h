#pragma once
#pragma warning(push)
#pragma warning(disable : 4514)
#pragma warning(disable : 4365)
#pragma warning(disable : 4820)
#include <DirectXTex.h>
#include <d3dx12.h>
#pragma warning(pop)
#include "MyMath.h"

/**
 * @file TextureManager.h
 * @brief テクスチャのマネージャー
 * @author 飯島 薫
 */

struct TextureData {
	// テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap;

	// GPUデスクリプタハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle{};

	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	// 横幅
	size_t width = 0;

	// 縦幅
	size_t height = 0;
};
namespace MesiEngine {
class TextureManager {
public: // 定数
	static const size_t MaxSRVCount = 256;

public:
	// 初期化
	void Init();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	TextureData LoadTexture(const std::string& fileName);

	TextureData LoadDivTexture(
	    const std::string& fileName_, KMyMath::Vector2 leftTop_, KMyMath::Vector2 divSize_);

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	static TextureData Load(const std::string& fileName);

	// 解放
	void Delete();

	// インスタンス
	static TextureManager* GetInstance();

	// ゲッター
	ID3D12DescriptorHeap* GetSrvHeap() { return srvHeap.Get(); }

private:
	/// <summary>
	/// テクスチャバッファの生成
	/// </summary>
	/// <param name="metadata"></param>
	/// <param name="scratchImg"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource>
	    CreateTexBuff(DirectX::TexMetadata& metadata, DirectX::ScratchImage& scratchImg);

	/// <summary>
	/// シェーダリソースビューの生成
	/// </summary>
	/// <param name="texBuff"></param>
	/// <param name="metadata"></param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE CreateSRV(ID3D12Resource* texBuff, DirectX::TexMetadata& metadata);

private:
	ID3D12Device* device = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
	D3D12_DESCRIPTOR_RANGE descriptorRange;
	D3D12_HEAP_PROPERTIES textureHeapProp{};

	// テクスチャバッファ
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, MaxSRVCount> texBuff_;

	// テクスチャ数
	UINT texCount;

	// デフォルトテクスチャ格納ディレクトリ
	static std::string DefaultTextureDirectoryPath;

	static TextureManager* textureManager;

private:
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
};
} // namespace MesiEngine