#include<windows.h>
#include<tchar.h>
#include "resource.h"
#include<commctrl.h>
#pragma comment(lib,"comctl32.lib")

BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ListCtrlProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
LPWSTR lpszClass = (LPWSTR)L"Dialog";

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	InitCommonControls();

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
	HWND hModaless;

	OPENFILENAME ofn;
	TCHAR strFile[MAX_PATH] = L"";
	TCHAR strTemp[MAX_PATH];

	static CHOOSEFONT font;
	static LOGFONT setfont;
	HFONT hFont, oldFont;
	TCHAR str[] = L"폰트 Test 1234";
	static bool fontflag = 0;

	static CHOOSECOLOR color;
	static COLORREF setColor;
	static COLORREF crCustColor[16];


	switch (iMessage)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		if (fontflag)
		{
			SetTextColor(hdc, color.rgbResult);
			hFont = CreateFontIndirect(&setfont);
			oldFont = (HFONT)SelectObject(hdc, hFont);

			TextOut(hdc, 100, 100, str, wcslen(str));

			SelectObject(hdc, oldFont);
			DeleteObject(hFont);
		}
		else
		{
			TextOut(hdc, 100, 100, str, wcslen(str));
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_MODAL:
			DialogBox(g_hInst, (LPWSTR)IDD_DIALOG, hWnd, DialogProc);
			break;
		case ID_MODALESS:
			hModaless = CreateDialog(g_hInst, (LPWSTR)IDD_DIALOG, hWnd, DialogProc);
			ShowWindow(hModaless, SW_SHOW);
			break;
		case ID_FILEOPEN:
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = L"Text File(*.txt)\0*.txt;\0";
			ofn.nMaxFile = MAX_PATH;
			if (GetOpenFileName(&ofn) != 0)
			{
				wsprintf(strTemp, L"%s 파일을 선택 했습니다.", ofn.lpstrFile);
				MessageBox(hWnd, strTemp, L"파일 열기", MB_OK);
			}
			break;
		case ID_FILESAVE:
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = L"Text File(*.txt)\0*.txt;\0";
			ofn.lpstrFile = strFile;
			ofn.nMaxFile = MAX_PATH;
			if (GetSaveFileName(&ofn) != 0)
			{
				wsprintf(strTemp, L"%s 파일을 선택 했습니다.", ofn.lpstrFile);
				MessageBox(hWnd, strTemp, L"파일 저장", MB_OK);
			}
			break;
		case ID_FONT:
			font.lStructSize = sizeof(CHOOSEFONT);
			font.hwndOwner = hWnd;
			font.lpLogFont = &setfont;
			font.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;

			if (ChooseFont(&font))
			{
				fontflag = 1;
				InvalidateRgn(hWnd, NULL, TRUE);
			}
			break;
		case ID_COLOR:
			setColor = RGB(0, 0, 0);
			color.hwndOwner = hWnd;
			color.lStructSize = sizeof(CHOOSECOLOR);
			color.rgbResult = setColor;
			color.Flags = CC_RGBINIT | CC_FULLOPEN;
			color.lpCustColors = crCustColor;
			if (ChooseColor(&color))
			{
				InvalidateRgn(hWnd, NULL, TRUE);
			}
			break;
		case ID_MENU_LISTCTRL:
			DialogBox(g_hInst, (LPWSTR)IDD_LISTCTRL, hWnd, ListCtrlProc);
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}


//핸들
HWND hList;
HWND hEdit;

BOOL CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR strEdit[128];	//버퍼
	switch (message)
	{
	case WM_INITDIALOG:
		hList = GetDlgItem(hDlg, IDC_LIST);
		hEdit = GetDlgItem(hDlg, IDC_EDIT);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, TRUE);
			break;
		case IDC_BTN_ADD:
			GetWindowText(hEdit, strEdit, wcslen(strEdit));
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)strEdit);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
	}
	return FALSE;
}


TCHAR*Columnlist[3] = { (TCHAR*)"이름",(TCHAR*)"주소" ,(TCHAR*)"휴대폰" };
int nWidth[3] = { 100,150,150 };
BOOL CALLBACK ListCtrlProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	//static HWND hListCtrl;
	//LVCOLUMN column;
	//LVITEM listItem;

	//switch

		return FALSE;
}