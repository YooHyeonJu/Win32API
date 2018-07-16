#include <windows.h>
#include <tchar.h>

#define ID_THREAD1 101
#define ID_THREAD2 102
#define ID_SUSPEND 103
#define ID_RESUME  104

HWND g_hWnd;
struct ThreadParam
{
	int x, y, w, h;
	int nSpeed;
}Param[2] = { { 100,100,200,200,10 },{ 320,100,200,200,30 } };

DWORD WINAPI ThreadFunc(LPVOID Param)
{
	HDC hdc;
	
	BYTE Blue = 0;

	HBRUSH hBrush, hOldBrush;
	//ThreadParam *P = (ThreadParam*)Param;
	hdc = GetDC(g_hWnd);
	for (;;)
	{
		Blue+=5;
		Sleep(20);		///1번 10ms, 2번 30ms(시간차)
		hBrush = CreateSolidBrush(RGB(0, 0, Blue));	//지정된 단색 브러시 초기화
		hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, 10,10,400,200);
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

	DWORD ThreadID;// [2];
	//DWORD dwRetCode;
	static HANDLE hThread;//[2];
	static int nThreadNum;

	switch (iMessage)
	{
	case WM_CREATE:
		g_hWnd = hWnd;
		
		hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID);
		CloseHandle(hThread);

		//CreateWindow(L"button", L"중지", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 220, 400, 80, 25, hWnd, (HMENU)ID_SUSPEND, g_hInst, NULL);
		//CreateWindow(L"button", L"실행", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 320, 400, 80, 25, hWnd, (HMENU)ID_RESUME, g_hInst, NULL);
		//CreateWindow(L"button", L"Thread0", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 200, 350, 100, 30, hWnd, (HMENU)ID_THREAD1, g_hInst, NULL);
		//CreateWindow(L"button", L"Thread1", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 320, 350, 100, 30, hWnd, (HMENU)ID_THREAD2, g_hInst, NULL);
		return 0;

	

	case WM_DESTROY:
		
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

