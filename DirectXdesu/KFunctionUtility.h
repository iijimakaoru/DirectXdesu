#pragma once
#include <string>
#include <vector>
#include <stringapiset.h>
#include <system_error>
#include <errhandlingapi.h>
#include <filesystem>
#include <iostream>
#include <compare>

namespace KFunctionUtility
{
	/// <summary>
	/// 文字列をワイド文字列に変換
	/// </summary>
	/// <param name="string">変換したい文字列</param>
	std::wstring StringToWstring(const std::string& string);

	/// <summary>
	/// ディレクトリからファイル名一覧を所得
	/// </summary>
	/// <param name="folderPath">ディレクトリパス</param>
	/// <returns>ファイル名一覧</returns>
	std::vector<std::string> getFileNames(const std::string& folderPath);

	/// <summary>
	/// ファイルパスからファイル名を取り出す
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <returns>ファイル名</returns>
	std::string GetFileName(std::string& path);

	/// <summary>
	/// ファイルパスからファイル名を取り出す(拡張子あり)
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <returns>ファイル名</returns>
	std::string ExtractFileName(const std::string& path);

	/// <summary>
	/// ファイルパスからディレクトリパスを取り出す
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <returns>ディレクトリパス</returns>
	std::string GetDirectoryPath(const std::string& filePath);

	/// <summary>
	/// ファイルパスから拡張子を取り出す
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <returns>拡張子</returns>
	std::string FileExtension(const std::string& path);

	// 拡張子を置き換える処理

	/// <summary>
	/// ファイルの拡張子を置き換える
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="ext">置き換える拡張子</param>
	std::string ReplaceExtension(const std::string& path, const char* ext);

	/// <summary>
	/// ファイルが存在するかどうか
	/// </summary>
	/// <param name="path">ファイルパス</param>
	bool FileIsExist(const std::string& path);
};

