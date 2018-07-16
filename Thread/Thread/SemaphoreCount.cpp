#include <windows.h>
#include <tchar.h>

HWND g_hWnd;
int g_nY = 0;
HANDLE hSemaphore;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"Semaphore";


DWORD WINAPI ThreadFunc(LPVOID Param)
{
	HDC hdc;
	TCHAR strText[128];
	int nSleep, nY, nPer = 0;

	nSleep = rand() % 5 + 1;
	hdc = GetDC(g_hWnd);
	g_nY += 20;
	nY = g_nY;

	TextOut(hdc, 10, nY, L"세마포어 대기중", 8);
	GdiFlush();			//현재 출력 버퍼의 내용을 출력해준다.
	WaitForSingleObject(hSemaphore, INFINITE);	//INFINITE가 끝날 때까지 무한 대기-> 출력을 위해 GdiFlush사용

	while (nPer <= 100)
	{
		wsprintf(strText, L"세마포어 수행중(%d%% 완료)", nPer);
		TextOut(hdc, 10, nY, strText, wcslen(strText));
		nPer++;
		Sleep(20 * nSleep);
	}
	ReleaseSemaphore(hSemaphore, 1, NULL);
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	HDC hdc;
	PAINTSTRUCT ps;
	DWORD ThreadID;
	HANDLE hThread;

	TCHAR Mes[] = L"마우스 왼쪽 버튼을 누르면 세마포어가 증가합니다.";

	switch (iMessage)
	{
	case WM_CREATE:
		g_hWnd = hWnd;
		hSemaphore = CreateSemaphore(NULL, 3, 3, NULL);	//세마포어의 카운트 최대값을 3으로 설정.
		return 0;

	case WM_LBUTTONDOWN:
		hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID);
		CloseHandle(hThread);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 10, 0, Mes, wcslen(Mes));
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}