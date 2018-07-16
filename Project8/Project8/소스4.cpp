#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "resource.h"

BOOL CALLBACK	DialogProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK 	WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ToolbarProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"ToolBar";

TBBUTTON tbb[8] = {
	0, ID_FILENEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0,
	1, ID_FILEOPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0,
	2, ID_FILESAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0,
	3, ID_EDITCUT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0,
	4, ID_EDITCOPY, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0,
	5, ID_EDITPASTE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0,
	6, ID_FILEPRINT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0,
	7, ID_HELPEXE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0,
};

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	//툴바와 상태바에 필요한 변수
	LPTBBUTTON	ptb;
	static HWND toolbar, statusbar, floatWnd;

	//풍선 도움말에 필요한 변수
	LPNMHDR		pnmh;
	LPTOOLTIPTEXT		lpToolTip;
	const wchar_t* pStrTooltipText = L"";
	TCHAR szTemp[20];
	UINT Idx;
	HMENU	hMenu;
	DWORD	style;

	switch (iMessage)
	{
	case WM_CREATE:
		ptb = tbb;
		//툴바 생성
		toolbar = CreateToolbarEx(hWnd, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_BORDER | CCS_TOP |
			TBSTYLE_TOOLTIPS, 1, 8, g_hInst, IDB_BITMAP, ptb, 7, 16, 16, 16, 15, sizeof(TBBUTTON));
		//상태바 생성
		statusbar = CreateStatusWindow(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CCS_BOTTOM, L"준비 되었습니다.",
			hWnd, 2);

		floatWnd = CreateDialog(g_hInst, (LPWSTR)IDD_FLOATTOOLBAR, hWnd, ToolbarProc);

		return 0;

	case WM_NOTIFY:
		pnmh = (LPNMHDR)lParam;
		//풍선 도움말이 필요하다면
		if (pnmh->code == TTN_NEEDTEXT)
		{
			lpToolTip = (LPTOOLTIPTEXT)lParam;
			//각 항목에 따라 풍선 도움말을 생성한다.
			switch (lpToolTip->hdr.idFrom)\
			{
			case ID_FILENEW:
				pStrTooltipText = L"새파일";
				break;
			case ID_FILEOPEN:
				pStrTooltipText = L"파일열기";
				break;
			case ID_FILESAVE:
				pStrTooltipText = L"파일저장";
				break;
			case ID_EDITCUT:
				pStrTooltipText = L"잘라내기";
				break;
			case ID_EDITCOPY:
				pStrTooltipText = L"복사";
				break;
			case ID_EDITPASTE:
				pStrTooltipText = L"붙여넣기";
				break;
			case ID_FILEPRINT:
				pStrTooltipText = L"인쇄";
				break;
			case ID_HELPEXE:
				pStrTooltipText = L"도움말";
				break;
			}
			wcscpy(lpToolTip->szText, pStrTooltipText);
			//상태바에 문자열 리소스의 데이터를 출력한다.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
			Idx = lpToolTip->hdr.idFrom;
			LoadString(g_hInst, Idx, szTemp, 80);
			SetWindowText(statusbar, szTemp);
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILENEW:
			MessageBox(hWnd, L"파일을 새로 만듭니다..", L"새파일", MB_OK);
			break;
		case ID_FILEOPEN:
			MessageBox(hWnd, L"기존의 파일을 엽니다.", L"파일열기", MB_OK);
			break;
		case ID_FILESAVE:
			MessageBox(hWnd, L"파일을 저장합니다..", L"파일저장", MB_OK);
			break;
		case ID_FILEPRINT:
			MessageBox(hWnd, L"파일을 출력합니다.", L"출력하기", MB_OK);
			break;
		case ID_EDITCUT:
			MessageBox(hWnd, L"문서를 잘라냅니다.", L"잘라내기", MB_OK);
			break;
		case ID_EDITCOPY:
			MessageBox(hWnd, L"문서를 복사합니다.", L"복사하기", MB_OK);
			break;
		case ID_EDITPASTE:
			MessageBox(hWnd, L"문서를 붙여넣기합니다.", L"붙여넣기", MB_OK);
			break;
		case ID_HELPEXE:
			MessageBox(hWnd, L"도움말을 출력합니다", L"도움말", MB_OK);
			break;

		case ID_FLOAT:
			hMenu = GetMenu(hWnd);
			EnableMenuItem(hMenu, ID_FLOAT, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, ID_DOCKING, MF_BYCOMMAND | MF_ENABLED);

			ShowWindow(toolbar, SW_HIDE);
			ShowWindow(floatWnd, SW_SHOW);
			break;
		case ID_DOCKING:
			ShowWindow(floatWnd, SW_HIDE);
			hMenu = GetMenu(hWnd);
			style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_BORDER | CCS_TOP | TBSTYLE_TOOLTIPS;
			ShowWindow(toolbar, SW_HIDE);
			SetWindowLong(toolbar, GWL_STYLE, (LONG)style);
			MoveWindow(toolbar, 0, 0, 0, 0, FALSE);
			ShowWindow(toolbar, SW_SHOW);

			EnableMenuItem(hMenu, ID_FLOAT, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, ID_DOCKING, MF_BYCOMMAND | MF_GRAYED);
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

BOOL CALLBACK ToolbarProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND toolbar;
	LPTBBUTTON ptb;
	LPNMHDR	pnmh;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		ptb = tbb;
		toolbar = CreateToolbarEx(hWnd, WS_CHILD | WS_VISIBLE | WS_BORDER | TBSTYLE_TOOLTIPS, 1, 8, g_hInst, IDB_BITMAP, ptb, 7, 16, 16, 16, 15, sizeof(TBBUTTON));
		return 0;
	case WM_NOTIFY:
		pnmh = (LPNMHDR)lParam;
		if (pnmh->code == TTN_NEEDTEXT)
		{
			SendMessage(GetParent(hWnd), iMsg, wParam, lParam);
			break;
		}
	case WM_COMMAND:
		SendMessage(GetParent(hWnd), iMsg, wParam, lParam);
		break;
	case WM_DESTROY:
		EndDialog(hWnd, TRUE);
		return 0;
	}

	return FALSE;
}