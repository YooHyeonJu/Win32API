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
}Param[2] = { {100,100,200,200,10},{320,100,200,200,30} };

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
		Red++;
		Green++;
		Blue++;
		Sleep(P->nSpeed);		///1�� 10ms, 2�� 30ms(�ð���)
		hBrush = CreateSolidBrush(RGB(Red, Green, Blue));	//������ �ܻ� �귯�� �ʱ�ȭ
		hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, P->x, P->y, P->x + P->w, P->y + P->h);
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

	DWORD ThreadID[2];
	//DWORD dwRetCode;
	static HANDLE hThread[2];
	static int nThreadNum;

	switch (iMessage)
	{
	case WM_CREATE:
		g_hWnd = hWnd;
		for(int i = 0;i<2;i++)
		{
			hThread[i] = CreateThread(NULL, 0, ThreadFunc, &Param[i], 0, &ThreadID[i]);
		}

		CreateWindow(L"button", L"����", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 220, 400, 80, 25, hWnd, (HMENU)ID_SUSPEND, g_hInst, NULL);
		CreateWindow(L"button", L"����", BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 320, 400, 80, 25, hWnd, (HMENU)ID_RESUME, g_hInst, NULL);
		CreateWindow(L"button", L"Thread0", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 200, 350, 100, 30, hWnd, (HMENU)ID_THREAD1, g_hInst, NULL);
		CreateWindow(L"button", L"Thread1", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 320, 350, 100, 30, hWnd, (HMENU)ID_THREAD2, g_hInst, NULL);
		return 0;

	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED)
		{
			switch (LOWORD(wParam))
			{
			case ID_THREAD1:
				nThreadNum = 0;
				break;
			case ID_THREAD2:
				nThreadNum = 1;
				break;
			case ID_SUSPEND:
				SuspendThread(hThread[nThreadNum]);
				break;
			case ID_RESUME:
				ResumeThread(hThread[nThreadNum]);
				break;
			}
		}
		return 0;

	case WM_DESTROY:
		for (int i = 0; i < 2; i++)
		{
			CloseHandle(hThread[i]);
			if (WaitForSingleObject(hThread, INFINITE))
				MessageBox(hWnd, L"������ ���� ����", L"�޽���", MB_OK);
		}
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

