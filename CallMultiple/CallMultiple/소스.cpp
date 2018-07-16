#include <windows.h>
#include <tchar.h>
#include "Declaredll.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"DLL_TEST";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	HDC hdc;
	TCHAR strResult[20];
	int nResult = 0;

	HINSTANCE hInstDll;
	int(*pMultiple)(int, int);

	//PAINTSTRUCT ps;
	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		
		/*
		//암시적 연결
		nResult = Multiple(20, 5);
		wsprintf(strResult, L"20 * 5 = %d", nResult);
		TextOut(hdc, 100, 100, strResult, wcslen(strResult));
		*/

		//명시적 연결 : LoadLibrary, GetProcAddress,FreeLibrary
		hInstDll = LoadLibrary(L"MultipleDll.dll");
		pMultiple = (int(*)(int, int))GetProcAddress(hInstDll, "Multiple");	//함수의 포인터 연산
		nResult = (*pMultiple)(20, 5);
		wsprintf(strResult, L"20 * 5 = %d", nResult);
		TextOut(hdc, 100, 100, strResult, wcslen(strResult));
		FreeLibrary(hInstDll);	//DLL 핸들을 해제하는 함수
		
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}