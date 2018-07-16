#include <windows.h>
#include <tchar.h>


HWND g_hWnd;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"ThreeSemaphore";
HANDLE hSemaphore;

DWORD WINAPI ThreadFunc(LPVOID Param)
{
	HDC hdc;
	TCHAR str[128];
	hdc = GetDC(g_hWnd);

	TCHAR Mes[] = L"마우스 왼쪽 버튼을 누르면 새 프로세스가 실행됩니다.";
	Sleep(10);
	
	TextOut(hdc, 50, 50, Mes, wcslen(Mes));
	ReleaseDC(g_hWnd, hdc);
	return 0;
}



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	hSemaphore = CreateSemaphore(NULL, 3, 3, L"Semaphore");
	if (WaitForSingleObject(hSemaphore, 0) == WAIT_TIMEOUT)
	{
		CloseHandle(hSemaphore);
		MessageBox(NULL, L"이미 세 개의 인스턴스가 실행중입니다.", L"메시지", MB_OK);
		return 0;
	}

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
	CloseHandle(hSemaphore);
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	HDC hdc;
	PAINTSTRUCT ps;

	DWORD ThreadID;
	HANDLE hThread;

	UINT nExeValue;
	TCHAR Mes[] = L"마우스 왼쪽 버튼을 누르면 새 프로세스가 실행됩니다.";

	switch (iMessage)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 50, 50, Mes, wcslen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID);/*
		nExeValue = WinExec("Semaphore", SW_SHOW);
		switch (nExeValue)
		{
		case ERROR_PATH_NOT_FOUND:
			MessageBox(NULL, L"지정한 경로를 찾을 수 없습니다.", L"메시지", MB_OK);
			break;
		case ERROR_FILE_NOT_FOUND:
			MessageBox(NULL, L"지정한 파일을 찾을 수 없습니다.", L"메시지", MB_OK);
			break;
		case ERROR_BAD_FORMAT:
			MessageBox(NULL, L".exe 파일이 유효하지 않습니다.", L"메시지", MB_OK);
			break;
		case 0:
			MessageBox(NULL, L"메모리를 초과하였습니다.", L"메시지", MB_OK);
			break;
		}*/
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}