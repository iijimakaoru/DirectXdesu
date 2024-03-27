#include "TextureManager.h"
#include "KDirectXCommon.h"

TextureManager* TextureManager::textureManager = nullptr;
std::string TextureManager::DefaultTextureDirectoryPath = "Resource/";

void TextureManager::Init() {
	HRESULT result;

	device = KDirectXCommon::GetInstance()->GetDev();

	// デスクリプタレンジの設定
	descriptorRange.NumDescriptors = 1; // 1度の描画に使うテクスチャの数
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0; // レジスタ番号
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// SRV用デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // シェーダーから見えるように
	srvHeapDesc.NumDescriptors = static_cast<UINT>(MaxSRVCount); // テクスチャの枚数
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap)); // 生成
	assert(SUCCEEDED(result));

	// ヒープ設定
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
}

TextureData TextureManager::LoadTexture(const std::string& fileName) {
	HRESULT result;

	if (texCount > 1024) {
		assert(0);
	}

	TextureData data{};

	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};
	DirectX::ScratchImage mipChain{};

	data.srvHeap = srvHeap;
	data.descriptorRange = descriptorRange;

	wchar_t wfilepath[256];

	MultiByteToWideChar(CP_ACP, 0, fileName.c_str(), -1, wfilepath, _countof(wfilepath));

	// テクスチャロード
	result = LoadFromWICFile(wfilepath, DirectX::WIC_FLAGS_NONE, &metadata, scratchImg);
	assert(SUCCEEDED(result));

	// ミニマップ作成
	result = GenerateMipMaps(
	    scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
	    DirectX::TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = DirectX::MakeSRGB(metadata.format);

	// テクスチャバッファの生成
	data.texBuff = CreateTexBuff(metadata, scratchImg);

	// シェーダリソースビューの生成
	data.gpuHandle = CreateSRV(data.texBuff.Get(), metadata);

	// 横幅記憶
	data.width = metadata.width;

	// 縦幅記憶
	data.height = metadata.height;

	texCount++;

	return data;
}

TextureData TextureManager::LoadDivTexture(
    const std::string& fileName_, KMyMath::Vector2 leftTop_, KMyMath::Vector2 divSize_) {
	HRESULT result;

	if (texCount > 1024) {
		assert(0);
	}

	TextureData data{};

	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};
	DirectX::ScratchImage mipChain{};

	data.srvHeap = srvHeap;
	data.descriptorRange = descriptorRange;

	wchar_t wfilepath[256];

	MultiByteToWideChar(CP_ACP, 0, fileName_.c_str(), -1, wfilepath, _countof(wfilepath));

	// テクスチャロード
	result = LoadFromWICFile(wfilepath, DirectX::WIC_FLAGS_NONE, &metadata, scratchImg);
	assert(SUCCEEDED(result));

	// ミニマップ作成
	result = GenerateMipMaps(
	    scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
	    DirectX::TEX_FILTER_DEFAULT, 0, mipChain);

	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// 読み込んだディフューズテクスチャをSRGBとして扱う
	metadata.format = DirectX::MakeSRGB(metadata.format);

	// テクスチャバッファの生成
	data.texBuff = CreateTexBuff(metadata, scratchImg);

	// シェーダリソースビューの生成
	data.gpuHandle = CreateSRV(data.texBuff.Get(), metadata);

	// 横幅記憶
	data.width = metadata.width;

	// 縦幅記憶
	data.height = metadata.height;

	texCount++;

	return data;
}

TextureData TextureManager::Load(const std::string& fileName) {
	return TextureManager::GetInstance()->LoadTexture(fileName);
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateTexBuff(
    DirectX::TexMetadata& metadata, DirectX::ScratchImage& scratchImg) {
	HRESULT result;

	Microsoft::WRL::ComPtr<ID3D12Resource> buff;

	// リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// テクスチャ用バッファの生成
	result = device->CreateCommittedResource(
	    &textureHeapProp, D3D12_HEAP_FLAG_NONE, &textureResourceDesc,
	    D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(buff.ReleaseAndGetAddressOf()));

	// 全ミニマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		// ミニマップレベルを指定してイメージを取得
		const DirectX::Image* img = scratchImg.GetImage(i, 0, 0);

		// テクスチャバッファにデータ転送
		result = buff->WriteToSubresource(
		    (UINT)i, nullptr, img->pixels, (UINT)img->rowPitch, (UINT)img->slicePitch);
		assert(SUCCEEDED(result));
	}

	return buff;
}

D3D12_GPU_DESCRIPTOR_HANDLE
TextureManager::CreateSRV(ID3D12Resource* texBuff, DirectX::TexMetadata& metadata) {
	// アドレスの先頭を取得
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

	UINT incrementSize =
	    device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// ハンドルのポインタずらし
	cpuHandle.ptr += static_cast<UINT64>(texCount) * incrementSize;
	gpuHandle.ptr += static_cast<UINT64>(texCount) * incrementSize;

	// シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = static_cast<UINT>(metadata.mipLevels);

	// ハンドルの指す位置にシェーダーリソースビュー作成
	device->CreateShaderResourceView(texBuff, &srvDesc, cpuHandle);

	return gpuHandle;
}

void TextureManager::Delete() { delete textureManager; }

TextureManager* TextureManager::GetInstance() {
	static TextureManager instance;
	return &instance;
}

void TextureManager::LoadTextures() {
	// タイトル
	textures["MESI"] = TextureManager::Load("Resources/texture/MESI.png");
	textures["SHOOTER"] = TextureManager::Load("Resources/texture/SHOOTER.png");
	textures["Mold"] = TextureManager::Load("Resources/texture/BattleShipMold.png");

	// ポーズ
	textures["Pose"] = TextureManager::Load("Resources/texture/pose.png");
	textures["Back"] = TextureManager::Load("Resources/texture/BackTitle.png");
	textures["Operation"] = TextureManager::Load("Resources/texture/Operation.png");

	// リザルト
	textures["Result"] = TextureManager::Load("Resources/texture/ResultText.png");
	textures["Bord"] = TextureManager::Load("Resources/texture/ScoreBord.png");
	textures["R_Back"] = TextureManager::Load("Resources/texture/ResultBack.png");
	textures["L_Score"] = TextureManager::Load("Resources/texture/LevelScore.png");
	textures["E_Bonus"] = TextureManager::Load("Resources/texture/EnemyBonus.png");
	textures["D_Bonus"] = TextureManager::Load("Resources/texture/MinDamageBonus.png");
	textures["T_Bonus"] = TextureManager::Load("Resources/texture/BossTimeBonus.png");
	textures["Total"] = TextureManager::Load("Resources/texture/Total.png");

	// プッシュAテクスチャ
	textures["PushA"] = TextureManager::Load("Resources/texture/kariNextScene.png");

	// 数字読み込み
	textures["0"] = TextureManager::Load("Resources/texture/Num0.png");
	textures["1"] = TextureManager::Load("Resources/texture/Num1.png");
	textures["2"] = TextureManager::Load("Resources/texture/Num2.png");
	textures["3"] = TextureManager::Load("Resources/texture/Num3.png");
	textures["4"] = TextureManager::Load("Resources/texture/Num4.png");
	textures["5"] = TextureManager::Load("Resources/texture/Num5.png");
	textures["6"] = TextureManager::Load("Resources/texture/Num6.png");
	textures["7"] = TextureManager::Load("Resources/texture/Num7.png");
	textures["8"] = TextureManager::Load("Resources/texture/Num8.png");
	textures["9"] = TextureManager::Load("Resources/texture/Num9.png");

	// 「Score」文字
	textures["Score"] = TextureManager::Load("Resources/texture/ScoreTex.png");

	// 「HIT」文字
	textures["Hit"] = TextureManager::Load("Resources/texture/HitTex.png");

	// 「X」文字
	textures["X"] = TextureManager::Load("Resources/texture/XTex.png");

	// ボーナス時間バースプライト
	textures["BonusBar"] = TextureManager::Load("Resources/texture/scoreBonusBar.png");

	// 万能1x1白テクスチャ
	textures["White1x1"] = TextureManager::Load("Resources/texture/white1x1.png");

	// キューブ用仮テクスチャ
	textures["Cube"] = TextureManager::Load("Resources/texture/kariPlayerColor.png");

	// プレイヤーのHPバー(枠)
	textures["PlayerHPBar"] = TextureManager::Load("Resources/texture/PlayerHPBar2.png");

	// ボムアイコン
	textures["BomIcon"] = TextureManager::Load("Resources/texture/BomIcon.png");

	// 操作説明
	textures["Setumei"] = TextureManager::Load("Resources/texture/setumei.png");
}

TextureData& TextureManager::GetTextures(std::string mapName) { return textures[mapName]; }

TextureData& TextureManager::GetNumTex(size_t i) {
	switch (i) {
	case 1:
		return GetTextures("1");
		break;
	case 2:
		return GetTextures("2");
		break;
	case 3:
		return GetTextures("3");
		break;
	case 4:
		return GetTextures("4");
		break;
	case 5:
		return GetTextures("5");
		break;
	case 6:
		return GetTextures("6");
		break;
	case 7:
		return GetTextures("7");
		break;
	case 8:
		return GetTextures("8");
		break;
	case 9:
		return GetTextures("9");
		break;
	default:
		return GetTextures("0");
		break;
	}
}
