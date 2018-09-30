#pragma once

#include "resource.h"
#include "ToolCommand.h"
#include "../CommonUtil/Math.h"

#define DLL_EXPORTS

#ifndef DLL_API
#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
#endif


extern "C"
{
	DLL_API bool Init(HWND hwnd, HINSTANCE hInstance);
	DLL_API int  UpdateLoop();
	DLL_API int  Command(ToolCommand command, void* input = nullptr, void* output = nullptr);
	DLL_API int  GetFPSInfo();
};
