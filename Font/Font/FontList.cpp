#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"FontList";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
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
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

#define ID_COMBOBOX 100

HWND	hCombo;
LOGFONT logfont[500];
int		nFontType[500];
int		nNum;
int		nCursel;
TCHAR	strFont[128];

int CALLBACK FontListCallBack(ENUMLOGFONT FAR *lpelf, NEWTEXTMETRIC FAR *lpntm, int FontType, LPARAM lParam)
{
	// 최대 500개까지의 폰트를 조사할 수 있다. 
	if (nNum < 500)
	{
		logfont[nNum] = lpelf->elfLogFont;
		nFontType[nNum] = FontType;
		nNum++;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HFONT	hFont, hOldFont;
	TCHAR strMes[128] = L"폰트를 출력합니다.";

	switch (iMessage)
	{
	case WM_CREATE:
		hCombo = CreateWindow(L"combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWN |
			WS_VSCROLL, 10, 50, 200, 500, hWnd, (HMENU)ID_COMBOBOX, g_hInst, NULL);
		hdc = GetDC(hWnd);
		EnumFontFamilies(hdc, NULL, (FONTENUMPROC)FontListCallBack, (LPARAM)NULL);
		ReleaseDC(hWnd, hdc);

		SendMessage(hCombo, CB_RESETCONTENT, 0, 0);

		for (int i = 0; i < nNum; i++)
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)logfont[i].lfFaceName);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_COMBOBOX:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				nCursel = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
				SendMessage(hCombo, CB_GETLBTEXT, nCursel, (LPARAM)strFont);
				SetWindowText(hWnd, strFont);
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		wcscpy(logfont[nCursel].lfFaceName, strFont);
		hFont = CreateFontIndirect(&logfont[nCursel]);
		hOldFont = (HFONT)SelectObject(hdc, hFont);

		TextOut(hdc, 300, 50, strMes, wcslen(strMes));
		SelectObject(hdc, hOldFont);
		DeleteObject(hFont);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}