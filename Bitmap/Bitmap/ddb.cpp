#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"Bitmap";

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

	HDC hdc,hMemDC;
	HBITMAP hBitmap, hOldBitmap;
	PAINTSTRUCT ps;

	BITMAP Bitmap;
	//BYTE Data[] = { 0xE7,0XFF,0XE7,0XFF,0XE7,0XFF,0X00,0XFF,0X00,0XFF,0XE7,0XFF,0XE7,0XFF,0XE7,0XFF };
	BYTE Data[] = { 0xE7,0X00,0XE7,0X00,0XE7,0X00,0X00,0X00,0X00,0X00,0XE7,0X00,0XE7,0X00,0XE7,0X00 };
	const wchar_t strMes[128] = L"래스터 데이터를 출력합니다.";

	switch (iMessage)
	{

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 50, 50, strMes, wcslen(strMes));
		hMemDC = CreateCompatibleDC(hdc);			//화면 메모리 제어 핸들
		hBitmap = CreateBitmap(8, 8, 1, 1, Data);	//8*8bit, 색상면의 수:흑백(1),색상결정 1Byte,래스터데이터 포인터(워드2byte단위)
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
		BitBlt(hdc, 50, 80, 8, 8, hMemDC, 0,0,SRCCOPY);	//SROCCOPY:복사
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
		DeleteObject(hBitmap);
		EndPaint(hWnd,&ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}