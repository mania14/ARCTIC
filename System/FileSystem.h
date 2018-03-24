#include "stdafx.h"
#include "../CommonUtil/Singletone.h"
#include <vector>

class FileSystem :public Singletone<FileSystem>
{
public:
	static void GetDirectoryFileList(const std::wstring& path, std::vector<std::wstring>& fileList, const std::wstring& filter = L"*.*");
	static std::string GetFileName(const std::string& FullPath);
	static std::wstring GetFileName(const std::wstring& FullPath);
	static std::string GetFileFormat(const std::string& FullPath);
	static std::wstring GetFileFormat(const std::wstring& FullPath);
	static std::string GetFilePath(const std::string& FullPath);
	static std::string SwitchFileFormat(const std::string& FullPath, const char* format);
};

class File
{
	FILE* pfile;
};