// Client.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <windowsx.h>
#include "Client.h"
#include "../System/RenderDevice.h"
#include "../System/InputManager.h"
#include "../System/GameTimer.h"
#include "../System/RandomManager.h"
#include "../GameEngine/PhysicsManager.h"
#include "../GameEngine/SceneManager.h"
#include "DirectXMath.h"
#include <sstream>
#include "../CommonUtil/CommonDefine.h"
#include "ToolCommand.h"

//#include "BoxScene.h"
//#include "AlphaScene.h"
//#include "MirrorScene.h"
//#include "TreeBillboardScene.h"
//#include "TestCalcShaderScene.h"
//#include "BlurScene.h"
#include "SkyBoxScene.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

												// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND g_hWnd;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	MSG msg = { 0 };

	   // �⺻ �޽��� �����Դϴ�.
	while (msg.message != WM_QUIT)
	{
		UpdateLoop();
	}

	RenderDevice::This().Release();


	return (int)msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CLIENT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!g_hWnd)
	{
		return FALSE;
	}

	if (FAILED(RenderDevice::This().InitDevice(g_hWnd)))
	{
		RenderDevice::This().Release();
		return FALSE;
	}

	SceneManager::This().InitScene(new SkyBoxScene());

	//���� Ÿ�̸�
	GameTimer::This().Reset();

	RandomManager::This().InitWELLRNG512a();

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
		break;

	case WM_EXITSIZEMOVE:
		RenderDevice::This().ResizeDevice();
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

bool Init(HWND hWnd, HINSTANCE hInstance)
{
	hInst = hInstance;
	g_hWnd = hWnd;

	if (!g_hWnd)
	{
		return FALSE;
	}

	if (FAILED(RenderDevice::This().InitDevice(g_hWnd)))
	{
		RenderDevice::This().Release();
		return FALSE;
	}

	PhysicsManager::This().InitPhysics();
	SceneManager::This().InitScene(new SkyBoxScene());

	//���� Ÿ�̸�
	RandomManager::This().InitWELLRNG512a();
	GameTimer::This().Reset();

	return TRUE;
}

int UpdateLoop()
{
	// �⺻ �޽��� �����Դϴ�.
	MSG msg = { 0 };
	std::wstring mMainWndCaption;

	{
		/*if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else*/
		{
			SceneManager::This().Update();
			SceneManager::This().Render();

			GameTimer::This().Tick();

			static int frameCnt = 0;
			static float timeElapsed = 0.0f;

			frameCnt++;

			// Compute averages over one second period.
			if ((GameTimer::This().TotalTime() - timeElapsed) >= 1.0f)
			{
				float fps = (float)frameCnt; // fps = frameCnt / 1
				float mspf = 1000.0f / fps;

				std::wostringstream outs;
				outs.precision(6);
				outs << mMainWndCaption << L"    "
					<< L"FPS: " << fps << L"    "
					<< L"Frame Time: " << mspf << L" (ms)";
				SetWindowText(g_hWnd, outs.str().c_str());

				// Reset for next average.
				frameCnt = 0;
				timeElapsed += 1.0f;
			}
		}
	}

	return 0;
}

DLL_API int Command(ToolCommand command, void * input, void * output)
{
	return ToolCommandManager::This().executeCommand(command, input, output);
}
