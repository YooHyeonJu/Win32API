#include <windows.h>
#include <tchar.h>
#include<wchar.h>


HWND g_hWnd;
int g_nY = 0;			//전역변수를 공유함.
HANDLE hSemaphore;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"ThreadSync";

DWORD WINAPI ThreadFunc1(LPVOID Param)
{
	HDC hdc;
	TCHAR str[128];

	hdc = GetDC(g_hWnd);

	WaitForSingleObject(hSemaphore, INFINITE);

	for (int i = 0; i <= 100; i++)
	{
		
		wsprintf(str, L"다운로드 중 : %d %%완료", i);
		g_nY = 100;
		Sleep(10);
		for (int j = 0; j < i; j++)
		{
			wcscat(str, L"|");
		}
		TextOut(hdc, 10, g_nY, str, wcslen(str));

		if (i >= 50)
		{
			if (i == 50)
			{
				for (int k = 0; k < i + 1; k++)
				{
					ReleaseSemaphore(hSemaphore, 1, NULL);
				}
					Sleep(100);
				
			}
			else
			{
				ReleaseSemaphore(hSemaphore, 1, NULL);
				Sleep(100);
			}
		}
		else
			Sleep(10);
	}
	ReleaseDC(g_hWnd, hdc);
	return 0;
}

DWORD WINAPI ThreadFunc2(LPVOID Param)
{
	HDC hdc;
	TCHAR str[128];

	hdc = GetDC(g_hWnd);
	for (int i = 0; i <= 100; i++)
	{
		WaitForSingleObject(hSemaphore, INFINITE);

		wsprintf(str, L"화면 출력 중 : %d %%완료", i);
		g_nY = 200;
		Sleep(10);
		for (int j = 0; j < i; j++)
		{
			wcscat(str, L"|");
		}
		TextOut(hdc, 10, g_nY, str, wcslen(str));
		ReleaseSemaphore(hSemaphore, 1, NULL);
		Sleep(100);
	}
	ReleaseDC(g_hWnd, hdc);
	return 0;
}



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


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	DWORD ThreadID;
	HANDLE hThread;

	switch (iMessage)
	{
	case WM_CREATE:
		g_hWnd = hWnd;
		hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
		return 0;
	case WM_LBUTTONDOWN:
		hThread = CreateThread(NULL, 0, ThreadFunc1, NULL, 0, &ThreadID);
		CloseHandle(hThread);
		hThread = CreateThread(NULL, 0, ThreadFunc2, NULL, 0, &ThreadID);
		CloseHandle(hThread);
		return 0;
	case WM_DESTROY:
		CloseHandle(hSemaphore);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}