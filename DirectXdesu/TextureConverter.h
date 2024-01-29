#pragma once
#include <DirectXTex.h>
#include <string>

class TextureConverter {
public:
	void ConvertTextureWICToDDS(const std::string& filePath);

private:
	void LoadWICTextureFromFile(const std::string& filePath);
	static std::wstring ConvertMultiByteStringToWideString(const std::string& filePath);
	void SeparateFilePath(const std::wstring& filePath);
	void SaveDDSTextureToFile();

private:
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage scratchImage;

	std::wstring directoryPath;
	std::wstring fileName;
	std::wstring fileExt;
};
