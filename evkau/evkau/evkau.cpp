// evkau.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "evkau.h"
#include "sp_updater.h"

#if USE_CURL
#include "curl.h"
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define WM_CMD_FROM_OTHER_PROCESS (WM_USER + 2)

#define MAX_LOADSTRING 64

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND	g_main_hwnd;

//int WINAPI WinMain(
//	HINSTANCE hInstance,
//	HINSTANCE hPrevInstance,
//	LPSTR lpCmdLine,
//	int nCmdShow
//)
//{
//
//	HMODULE lib = LoadLibrary(L"User32.dll");
//	if (lib)
//	{
//		typedef BOOL(*pFunSetProcessDPIAware)();
//		pFunSetProcessDPIAware fSetProcessDPIAware = (pFunSetProcessDPIAware)GetProcAddress(lib, "SetProcessDPIAware");
//		if (fSetProcessDPIAware)
//		{
//			fSetProcessDPIAware();
//		}
//	}
//
//	UNREFERENCED_PARAMETER(hPrevInstance);
//	UNREFERENCED_PARAMETER(lpCmdLine);
//
//	// TODO: Place code here.
//
//	// Initialize global strings
//	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//	LoadStringW(hInstance, IDC_EVKAU, szWindowClass, MAX_LOADSTRING);
//
//	if (FindWindowW(szWindowClass, NULL))
//		return 0;
//
//	MyRegisterClass(hInstance);
//
//	// Perform application initialization:
//	if (!InitInstance(hInstance, nCmdShow)) {
//		return FALSE;
//	}
//
//	std::wstring command(lpCmdLine, lpCmdLine + strlen(lpCmdLine));
//
//	sp_updater::action(hInstance, g_main_hwnd, (LPWSTR)command.c_str());
//
//	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EVKAU));
//	MSG msg;
//	while (GetMessage(&msg, nullptr, 0, 0)) {
//		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//	}
//
//	return (int)msg.wParam;
//
//}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int  nCmdShow)
{
	HMODULE lib = LoadLibrary(L"User32.dll");
	if (lib)
	{
		typedef BOOL(*pFunSetProcessDPIAware)();
		pFunSetProcessDPIAware fSetProcessDPIAware = (pFunSetProcessDPIAware)GetProcAddress(lib, "SetProcessDPIAware");
		if (fSetProcessDPIAware)
		{
			fSetProcessDPIAware();
		}
	}

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_EVKAU, szWindowClass, MAX_LOADSTRING);

	if (FindWindowW(szWindowClass, NULL))
		return 0;

	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	//std::wstring command(lpCmdLine, lpCmdLine + strlen(lpCmdLine));

	sp_updater::action(hInstance, g_main_hwnd, lpCmdLine);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EVKAU));
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EVKAU));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_EVKAU);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

extern "C" BOOL(STDAPICALLTYPE *pChangeWindowMessageFilter)(UINT, DWORD) = NULL;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	g_main_hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!g_main_hwnd)
	{
		return FALSE;
	}

	ShowWindow(g_main_hwnd, SW_HIDE);
	UpdateWindow(g_main_hwnd);

	HMODULE hMod = 0;
	if ( (hMod = ::LoadLibrary(L"user32.dll")) != 0) 
	{
		pChangeWindowMessageFilter = (BOOL(__stdcall *)(UINT, DWORD))::GetProcAddress(hMod, "ChangeWindowMessageFilter");

		if (pChangeWindowMessageFilter) 
		{
			pChangeWindowMessageFilter(WM_CMD_FROM_OTHER_PROCESS, MSGFLT_ADD);
		}
	}

	return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
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
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CMD_FROM_OTHER_PROCESS:
	{
		if (wParam == 1000)
		{
			DestroyWindow(hWnd);
		}
		break;
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
