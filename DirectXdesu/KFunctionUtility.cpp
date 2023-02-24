#include "KFunctionUtility.h"

namespace KFunctionUtility
{
    std::wstring StringToWstring(const std::string& string)
    {
        auto const dest_size = ::MultiByteToWideChar(CP_ACP, 0U, string.data(), -1, nullptr, 0U);
        std::vector<wchar_t> dest(static_cast<const unsigned _int64>(dest_size), L'\0');
        if (::MultiByteToWideChar(CP_ACP, 0U, string.data(), -1, dest.data(), static_cast<int>(dest.size())) == 0)
        {
            throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
        }
        dest.resize(std::char_traits<wchar_t>::length(dest.data()));
        dest.shrink_to_fit();
        return std::wstring(dest.begin(), dest.end());
    }

    // ディレクトリからファイル名一覧を所得
    std::vector<std::string> getFileNames(const std::string& folderPath)
    {
        std::filesystem::directory_iterator iter(folderPath), end;
        std::error_code err;
        std::vector<std::string> file_names;

        for (; iter != end && !err; iter.increment(err))
        {
            const std::filesystem::directory_entry entry = *iter;

            file_names.push_back(entry.path().string());
        }

        return file_names;
    }

	std::string GetFileName(std::string& path)
	{
		std::filesystem::path filePath = path;

		return filePath.stem().string();
	}

	std::string ExtractFileName(const std::string& path)
	{
		size_t pos1 = 0;

		//区切り文字'\\'が出てくる一番最後の部分を検索
		pos1 = path.rfind('\\');
		if (pos1 != std::string::npos)
		{
			return path.substr(pos1 + 1, path.size() - pos1 - 1);
		}

		//区切り文字'\'が出てくる一番最後の部分を検索
		pos1 = path.rfind('/');
		if (pos1 != std::string::npos)
		{
			return path.substr(pos1 + 1, path.size() - pos1 - 1);
		}

		return path;
	}

	std::string GetDirectoryPath(const std::string& filePath)
	{
		std::filesystem::path path = filePath;
		return path.remove_filename().string();
	}

	std::string FileExtension(const std::string& path)
	{
		size_t idx = path.rfind(L'.');
		return path.substr(idx + 1, path.length() - idx - 1);
	}

	std::string ReplaceExtension(const std::string& path, const char* ext)
	{
		std::filesystem::path filePath = path.c_str();
		return filePath.replace_extension(ext).string();
	}

	bool FileIsExist(const std::string& path)
	{
		FILE* fp;

		fopen_s(&fp, path.c_str(), "r");

		if (fp == NULL)
		{
			printf("%sは存在しません。\n", path.c_str());

			fclose(fp);

			return false;

		}

		fclose(fp);

		return true;
	}
};
