#include <windows.h>
#include <tchar.h>

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
	static TCHAR strChar[MAX_PATH];
	int nlen = 0;

	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CHAR:
		nlen = strlen((const char *)strChar);
		strChar[nlen] = (char)wParam;
		strChar[nlen + 1] = 0;
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 100, strChar, strlen((const char *)strChar));
		EndPaint(hWnd, &ps);
		return 0;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_HOME:
			MessageBox(hWnd, L"Home 키를 눌렀습니다.", L"메시지 박스", MB_OK);
			return 0;
		case VK_END:
			MessageBox(hWnd, L"End 키를 눌렀습니다.", L"메시지 박스", MB_OK);
			return 0;
		case VK_F1:
			MessageBox(hWnd, L"F1 키를 눌렀습니다.", L"메시지 박스", MB_OK);
			return 0;
		case VK_UP:
			MessageBox(hWnd, L"위쪽 방향키를 눌렀습니다.", L"메시지 박스", MB_OK);
			return 0;
		}
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}