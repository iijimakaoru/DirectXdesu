#include "TextureConverter.h"
#include <stringapiset.h>

void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath) {
	LoadWICTextureFromFile(filePath);

	SaveDDSTextureToFile();
}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath) {
	HRESULT result;

	std::wstring wfilePath = ConvertMultiByteStringToWideString(filePath);

	result = DirectX::LoadFromWICFile(
	    wfilePath.c_str(), DirectX::WIC_FLAGS_NONE, &metadata, scratchImage);
	assert(SUCCEEDED(result));

	SeparateFilePath(wfilePath);
}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString) {
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

	std::wstring wString;
	wString.resize(filePathBufferSize);

	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}

void TextureConverter::SeparateFilePath(const std::wstring& filePath) {
	size_t pos1;
	std::wstring exceptExt;

	pos1 = filePath.rfind(',');

	if (pos1 != std::wstring::npos) {
		fileExt = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		exceptExt = filePath.substr(0, pos1);
	} else {
		fileExt = L"";
		exceptExt = filePath;
	}

	pos1 = exceptExt.rfind('\\');
	if (pos1 != std::wstring::npos) {
		directoryPath = exceptExt.substr(0, pos1 + 1);
		fileName = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos) {
		directoryPath = exceptExt.substr(0, pos1 + 1);
		fileName = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	directoryPath = L"";
	fileName = exceptExt;
}

void TextureConverter::SaveDDSTextureToFile() {
	metadata.format = DirectX::MakeSRGB(metadata.format);

	HRESULT result;

	std::wstring filePath = directoryPath + L".dds";

	result = DirectX::SaveToDDSFile(
	    scratchImage.GetImages(), scratchImage.GetImageCount(), metadata, DirectX::DDS_FLAGS_NONE,
	    filePath.c_str());
	assert(SUCCEEDED(result));
}