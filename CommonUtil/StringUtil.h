#include <atlconv.h>
#include <string>
#include <algorithm>

//��Ƽ����Ʈ ���ڿ��� �����ڵ� ���ڿ��� ��ȯ
static std::wstring Multi2Wide(const char* pStr)
{
	WCHAR temp[1024] = L"";
	int len = -1;

	int nChars = MultiByteToWideChar(CP_ACP, 0, pStr, len, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, pStr, len, temp, nChars);

	return std::wstring(temp);
}

//�����ڵ� ���ڿ��� ��Ƽ����Ʈ ���ڿ��� ��ȯ
static std::string Wide2Multi(const wchar_t* pStr)
{
	CHAR temp[1024] = "";
	int len = -1;

	int nChars = WideCharToMultiByte(CP_ACP, 0,
		pStr, len, NULL, 0, NULL, NULL);

	WideCharToMultiByte(CP_ACP, 0, pStr, len,
		temp, nChars, NULL, NULL);

	return std::string(temp);
}

static void MakeStringLower(std::string& src)
{
	std::transform(src.begin(), src.end(), src.begin(), ::tolower);
}

//static std::string MakeStringLowerValue(std::string& src)
//{
//	std::string temp = src;
//	//MakeStringLower(temp);
//
//	return temp;
//}