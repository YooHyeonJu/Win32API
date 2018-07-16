#include <windows.h>
#include <tchar.h>
#include "resource.h"
#pragma comment(lib,"msimg32.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"BITMAP";

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

	HDC hdc, hMemDC;
	HBITMAP hBitmap, hOldBitmap;
	PAINTSTRUCT ps;
	BITMAP Bitmap;
	int nWidth, nHeight;

	switch (iMessage)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);		//ȭ�� DC
		hMemDC = CreateCompatibleDC(hdc);	//�޸� DC�� �����.(ȭ�� DC�� �ٷ� ����ϸ�, �������� ����)
		hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

		GetObject(hBitmap, sizeof(BITMAP), &Bitmap);

		nWidth = Bitmap.bmWidth;
		nHeight = Bitmap.bmHeight;

		//BitBlt(hdc, 0, 0, nWidth/2, nHeight/2, hMemDC, nWidth/2, nHeight/2, SRCCOPY);	//����� ȭ�� DC�� ����(��Ʈ�� �����ġ,����)
		//������ ���� ����
		StretchBlt(hdc, 0, 0, nWidth, nHeight, hMemDC, 0, 0, nWidth, nHeight, SRCCOPY);
		//2�� Ȯ��
		//StretchBlt(hdc, 0, 0, nWidth*2, nHeight*2, hMemDC, 0, 0, nWidth, nHeight, SRCCOPY);
		//2�� ���
		//SetStretchBltMode(hdc, COLORONCOLOR);
		//StretchBlt(hdc, 0, 0, nWidth *0.8, nHeight *0.8, hMemDC, 0, 0, nWidth, nHeight, SRCCOPY);	//�߰� 0,0 : ��Ʈ�� ��ü ����

		//TransparentBlt(hdc, 0, 0, nWidth, nHeight, hMemDC, 0, 0, nWidth, nHeight, RGB(0, 255, 0));

		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
		DeleteObject(hBitmap);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}