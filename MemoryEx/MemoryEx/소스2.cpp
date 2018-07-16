#include <windows.h>
#include <tchar.h>

#define  HeapInitSize 4096
#define HeapMaxSize 4096*256

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
	TCHAR strAlloc[128] = L"사용자 힙 메모리를 할당 하였습니다.";
	TCHAR strData[128];
	static int *ptr;
	HANDLE MyHeap;

	switch (iMessage)
	{

	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);

		MyHeap = HeapCreate(0, HeapInitSize, HeapMaxSize);

		//직접 힙 핸들을 구함.
		ptr = (int*)HeapAlloc(MyHeap, 0, sizeof(int) * 10);

		if (ptr != NULL)	TextOut(hdc, 10, 20, strAlloc, wcslen(strAlloc));

		for (int i = 0; i < 10; i++)
		{
			ptr[i] = i * 2;
		}

		for (int i = 0; i < 10; i++)
		{
			_itow(ptr[i], strData, 10);
			TextOut(hdc, 20 * i, 40, strData, wcslen(strData));
		}

		
		HeapFree(MyHeap, 0, ptr);
		HeapDestroy(MyHeap);
		//사용이 끝나면 힙을 해제시킨다. 메모리 확정을 해제하고, 힙을 무효화시킨다.

		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}