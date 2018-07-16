#include <windows.h>
#include <tchar.h>
#include <wchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"Input";

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
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
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
	static int nX = 0, nY = 0;
	static TCHAR strTemp[MAX_PATH];
	RECT rc;

	switch (iMessage)
	{
	case WM_CREATE:
		wcscpy(strTemp, L"");
		return 0;

	case WM_LBUTTONDOWN:
		SetCapture(hWnd);
		nX = LOWORD(lParam);
		nY = HIWORD(lParam);
		wsprintf(strTemp, L"좌표(%d, %d), 키보드 조합상태: %d", nX, nY, wParam);
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_MOUSEMOVE:
		if (GetCapture() == hWnd) {
			nX = LOWORD(lParam);
			nY = HIWORD(lParam);
			wsprintf(strTemp, L"좌표(%d, %d), 키보드 조합상태: %d", nX, nY, wParam);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		return 0;
		
		/*
	case WM_LBUTTONDBLCLK:	//항상 WM_LBUTTONDOWN을 거친다.
		hdc = GetDC(hWnd);
		MoveToEx(hdc, nX, nY + 15, NULL);	//선을 그어준다.(밑줄)
		LineTo(hdc, nX + 240, nY + 15);
		ReleaseDC(hWnd, hdc);
		return 0;
		*/
	case WM_LBUTTONUP:
		ReleaseCapture();
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetWindowRect(hWnd, &rc);
		SetTextAlign(hdc, TA_CENTER);
		TextOut(hdc,(rc.right - rc.left)/2, (rc.bottom - rc.top)/2, strTemp, wcslen(strTemp));
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}