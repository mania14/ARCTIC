#include "FileSystem.h"
#include <stdio.h>
#include <io.h>
#include "../CommonUtil/StringUtil.h"

void FileSystem::GetDirectoryFileList(const std::wstring& path, std::vector<std::wstring>& fileList, const std::wstring& filter)
{	
	std::wstring searching = path + filter;

	_finddata_t fd;
	intptr_t handle = _findfirst(Wide2Multi(searching.c_str()).c_str(), &fd);  //현재 폴더 내 모든 파일을 찾는다.

	if (handle == -1)    return;

	int result = 0;
	do
	{
		fileList.push_back(path + Multi2Wide(fd.name));
		result = _findnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);

	return;
}

std::string FileSystem::GetFileName(const std::string & FullPath)
{
	char FileName[_MAX_FNAME];
	_splitpath_s(FullPath.c_str(), nullptr, 0, nullptr, 0, FileName, _MAX_FNAME, nullptr, 0);

	return std::string(FileName);
}

std::wstring FileSystem::GetFileName(const std::wstring & FullPath)
{
	char FileName[_MAX_FNAME];
	_splitpath_s( Wide2Multi(FullPath.c_str()).c_str(), nullptr, 0, nullptr, 0, FileName, _MAX_FNAME, nullptr, 0);

	return std::wstring(Multi2Wide(FileName).c_str());
}

std::string FileSystem::GetFileFormat(const std::string & FullPath)
{
	char FormatName[_MAX_FNAME];
	_splitpath_s(FullPath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, FormatName, _MAX_FNAME);

	return std::string(FormatName);
}

std::wstring FileSystem::GetFileFormat(const std::wstring & FullPath)
{
	char FormatName[_MAX_FNAME];
	_splitpath_s(Wide2Multi(FullPath.c_str()).c_str(), nullptr, 0, nullptr, 0, nullptr, 0, FormatName, _MAX_FNAME);

	return std::wstring(Multi2Wide(FormatName).c_str());
}

std::string FileSystem::GetFilePath(const std::string & FullPath)
{
	char NewPath[_MAX_PATH] = { 0 };
	char DriveName[_MAX_DRIVE] = { 0 };
	char DirName[_MAX_DIR] = { 0 };
	
	_splitpath_s(FullPath.c_str(), DriveName, _MAX_DRIVE, DirName, _MAX_DIR, nullptr, 0, nullptr, 0);
	sprintf_s(NewPath, _MAX_PATH, "%s%s", DriveName, DirName);

	return std::string(NewPath);
}

std::string FileSystem::SwitchFileFormat(const std::string & FullPath, const char * format)
{
	char NewPath[_MAX_PATH] = {0};
	char DriveName[_MAX_DRIVE] = { 0 };
	char DirName[_MAX_DIR] = { 0 };
	char FileName[_MAX_FNAME] = { 0 };
	_splitpath_s(FullPath.c_str(), DriveName, _MAX_DRIVE, DirName, _MAX_DIR, FileName, _MAX_FNAME, nullptr, 0);
	sprintf_s(NewPath, _MAX_PATH, "%s%s%s%s", DriveName, DirName, FileName, format);

	return std::string(NewPath);
}
