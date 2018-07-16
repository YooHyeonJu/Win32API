#include <windows.h>
#include <tchar.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"Bitmap(coffee)";

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

	HDC hdc,hMemDC,hEmfDC;
	PAINTSTRUCT ps;
	RECT rt;

	BITMAP bm;
	HBITMAP hBm;
	HENHMETAFILE hEmf;

	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		hEmfDC = CreateEnhMetaFile(hdc, L"BitmapMetaFile.emf", NULL, NULL);
		hBm = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_COFFEE));
		GetObject(hBm, sizeof(BITMAP), &bm);
		hMemDC = CreateCompatibleDC(hEmfDC);
		SelectObject(hMemDC, hBm);
		StretchBlt(hEmfDC, 100, 100, 100, 100, hMemDC, 0, 0, bm.bmWidth,bm.bmHeight,SRCCOPY);
		DeleteDC(hMemDC);
		hEmf = CloseEnhMetaFile(hEmfDC);
		DeleteEnhMetaFile(hEmf);
		ReleaseDC(hWnd, hdc);
		return 0;

	case WM_RBUTTONDOWN:
		hdc = GetDC(hWnd);
		hEmf = GetEnhMetaFile(L"BitmapMetaFile.emf");
		if (hEmf == NULL)
		{
			MessageBox(hWnd, L"파일을 찾을 수 없습니다.", L"알림", MB_OK);
		}
		else
		{
			rt.left = LOWORD(lParam);
			rt.right = rt.left + 100;
			rt.top = HIWORD(lParam);
			rt.bottom = rt.top + 100;
			PlayEnhMetaFile(hdc, hEmf, &rt);
		}
		PlayEnhMetaFile(hdc, hEmf, &rt);
		DeleteEnhMetaFile(hEmf);
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}