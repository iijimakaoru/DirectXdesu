#pragma once
#include <d3d12.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <DirectXTex.h>

#include "KUtility.h"
#include "KDirectXCommon.h"

class TextureManager
{
private:

	KDirectXCommon* dx;

	static TextureManager* textureManager;

	//テクスチャ数
	uint32_t nextTexture;

	// ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};

	enum ImgFileType
	{
		WIC,//pngなど
		TGA,//tgaなど
		PSD,//psdなど
		ETC,//それ以外
	};

	static std::vector<std::string>filePaths;

	static std::unordered_map<std::string, std::unique_ptr<KTextureData>> textureDatas;

public:

	/// <summary>
	/// テクスチャをロードします
	/// </summary>
	/// <param name="filepath">テクスチャのファイルパス</param>
	/// <returns>テクスチャハンドル</returns>
	uint32_t LoadTexture(const std::string& path);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// インスタンスを所得
	/// </summary>
	/// <returns>インスタンス</returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// インスタンスを解放
	/// </summary>
	void Destroy();

	/// <summary>
	/// テクスチャをロードします
	/// </summary>
	/// <param name="filepath">テクスチャのファイルパス</param>
	/// <returns>テクスチャハンドル</returns>
	static uint32_t Load(const std::string& path);

	static KTextureData* GetTextureData(uint32_t handle);

private:

	TextureManager() = default;
	~TextureManager() = default;

	Microsoft::WRL::ComPtr<ID3D12Resource>CreateTexBuff(DirectX::TexMetadata & metadata, DirectX::ScratchImage & scratchImg);

	D3D12_GPU_DESCRIPTOR_HANDLE CreateShaderResourceView(ID3D12Resource * texBuff, DirectX::TexMetadata & metadata);

	void LoadFile(const std::string & path, DirectX::TexMetadata & metadata, DirectX::ScratchImage & scratchImg);

	KTextureData* FromTextureData(const std::string & path);

	ImgFileType GetFileType(const std::string & path);
};

