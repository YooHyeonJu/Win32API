#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"Mutex";
HANDLE hMutex;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	hMutex = CreateMutex(NULL, FALSE, L"Mutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, L"�̹� �ٸ� �ν��Ͻ��� �������Դϴ�.", L"�޽���", MB_OK);
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
	CloseHandle(hMutex);
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	HDC hdc;
	PAINTSTRUCT ps;
	
	UINT nExeValue;
	TCHAR Mes[] = L"���콺 ���� ��ư�� ������ �� ���μ����� ����˴ϴ�.";

	switch (iMessage)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 50, 50, Mes, wcslen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		nExeValue = WinExec("Mutex", SW_SHOW);
		switch (nExeValue)
		{
		case ERROR_PATH_NOT_FOUND:
			MessageBox(NULL, L"������ ��θ� ã�� �� �����ϴ�.", L"�޽���", MB_OK);
			break;
		case ERROR_FILE_NOT_FOUND:
			MessageBox(NULL, L"������ ������ ã�� �� �����ϴ�.", L"�޽���", MB_OK);
			break;
		case ERROR_BAD_FORMAT:
			MessageBox(NULL, L".exe ������ ��ȿ���� �ʽ��ϴ�.", L"�޽���", MB_OK);
			break;
		case 0:
			MessageBox(NULL, L"�޸𸮸� �ʰ��Ͽ����ϴ�.", L"�޽���", MB_OK);
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}