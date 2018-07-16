#include <windows.h>
#include <tchar.h>

HWND g_hWnd;
struct ThreadParam
{
	int x, y, w, h;
}Param[4] = { { 100,100,200,200 },{ 300,100,400,200 },{ 100,300,200,400 },{ 300,300,400,400 } };

DWORD WINAPI ThreadFunc(LPVOID Param)
{
	HDC hdc;

	BYTE Red = 0;
	BYTE Green = 0;
	BYTE Blue = 0;


	HBRUSH hBrush, hOldBrush;
	ThreadParam *P = (ThreadParam*)Param;
	hdc = GetDC(g_hWnd);
	for (;;)
	{
		Blue += 5;
		Sleep(20);
		hBrush = CreateSolidBrush(RGB(5, 5, Blue));
		hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, P->x, P->y, P->w, P->h);
		SelectObject(hdc, hOldBrush);
		DeleteObject(hBrush);
	}
	ReleaseDC(g_hWnd, hdc);
	return 0;
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"MULTI_THREAD";

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
	DWORD ThreadID[4];
	//DWORD dwRetCode;
	static HANDLE hThread[4];
	static int nThreadNum;
	int i;

	switch (iMessage)
	{
	case WM_CREATE:
		g_hWnd = hWnd;
		for (i = 0; i < 4; i++)
		{
			hThread[i] = CreateThread(NULL, 0, ThreadFunc, &Param[i], 0, &ThreadID[i]);
		}

		return 0;

	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		Ellipse(hdc, LOWORD(lParam) - 10, HIWORD(lParam) - 10, LOWORD(lParam) + 10, HIWORD(lParam) + 10);
		ReleaseDC(hWnd, hdc);
		return 0;

	case WM_DESTROY:
		for (i = 0; i < 4; i++)
		{
			CloseHandle(hThread[i]);
		}
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

