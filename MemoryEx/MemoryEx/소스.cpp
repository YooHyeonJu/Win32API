#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"MemoryEx";

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
	TCHAR strAlloc[128] = L"가상메모리를 할당 하였습니다.";
	TCHAR strFree[128] = L"가상메모리를 해제 하였습니다.";
	BOOL bMemFree;
	static TCHAR*ptr;
	RECT rect = { 10,40,100,60 };

	//PAINTSTRUCT ps;
	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		//가상 메모리 할당 ->페이지 단위 할당 (4096)
		ptr = (TCHAR*)VirtualAlloc(NULL, sizeof(TCHAR) * 10, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		if (ptr != NULL)	TextOut(hdc, 10, 20, strAlloc, wcslen(strAlloc));

		for (int i = 0; i < 10; i++)
		{
			ptr[i] = 'a' + i;
		}
		TextOut(hdc, 10, 40, ptr, wcslen(ptr));

		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_RBUTTONDOWN:
		hdc = GetDC(hWnd);
		//가상메모리 할당 해제
		bMemFree = VirtualFree(ptr, sizeof(TCHAR) * 10, MEM_DECOMMIT);

		if (bMemFree != 0)	TextOut(hdc, 10, 20, strFree, wcslen(strFree));

		InvalidateRect(hWnd, &rect, TRUE);
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}