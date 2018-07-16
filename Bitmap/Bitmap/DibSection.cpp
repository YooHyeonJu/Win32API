#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPWSTR lpszClass = (LPWSTR)L"DIBSection";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
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
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

HBITMAP MakeDIBSection()
{
	HANDLE hFile;
	DWORD dwFileSize, dwRead;
	BITMAPFILEHEADER fileheader;
	HBITMAP hBitmap;
	BITMAPINFO *info;
	PVOID pData;

	hFile = CreateFile(L"mfc.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}
	ReadFile(hFile, &fileheader, sizeof(BITMAPFILEHEADER), &dwRead, NULL);
	dwFileSize = fileheader.bfOffBits - sizeof(BITMAPFILEHEADER);
	info = (BITMAPINFO *)malloc(dwFileSize);
	ReadFile(hFile, info, dwFileSize, &dwRead, NULL);
	hBitmap = CreateDIBSection(NULL, info, DIB_RGB_COLORS, &pData, NULL, 0);
	ReadFile(hFile, pData, fileheader.bfSize - fileheader.bfOffBits, &dwRead, NULL);
	free(info);
	CloseHandle(hFile);
	return hBitmap;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, hMemDC;
	PAINTSTRUCT ps;
	static HBITMAP hBitmap;
	HBITMAP hOldBitmap;
	BITMAP	bit;

	switch (iMessage)
	{
	case WM_CREATE:
		hBitmap = MakeDIBSection();
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hMemDC = CreateCompatibleDC(hdc);
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
		GetObject(hBitmap, sizeof(BITMAP), &bit);
		BitBlt(hdc, 100, 100, bit.bmWidth, bit.bmHeight, hMemDC, 0, 0, SRCCOPY);
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}