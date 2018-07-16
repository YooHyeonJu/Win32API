#include <windows.h>
#include <tchar.h>

#define WM_IPC_MSG	WM_USER + 1

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"MsgServer";

int num = 65;



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
	HWND hFindWindow;
	const wchar_t strMsg[] =  L"���콺 ���� ��ư�� ������ Ŭ���̾�Ʈ ���μ����� �޽����� ���۵˴ϴ�." ;


	switch (iMessage)
	{

	case WM_LBUTTONDOWN:
		hFindWindow = FindWindow(NULL, L"MsgClient");
		//lpClassName,lpWindowName���� �����츦 ã�´�
		if (hFindWindow != NULL)
		{
			SendMessage(hFindWindow, WM_IPC_MSG, (WPARAM)num, (LPARAM)0);
			//������� �޽����� �����Ѵ�.
			num++;
		}
		else
			MessageBox(hWnd, L"Ŭ���̾�Ʈ�� ã�� �� �����ϴ�.", L"����", MB_OK);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 100, strMsg, wcslen(strMsg));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

