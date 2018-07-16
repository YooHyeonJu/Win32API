#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "resource.h"
#pragma comment(lib,"commctl32.lib")
//#pragma comment(lib, "comctl32.lib")

BOOL CALLBACK		DialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK		ListCtrlProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK		TreeCtrlProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK		EtcCtrlProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"Dialog";


//��ȭ ���� ��ݿ��� WinMain, WndProc�� �߰��Ͽ�, ������ ������� ������.(��� ���)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{

	///�⺻ ������ ����
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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);	//�⺻ ������ ���� �� �޴� ���
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&WndClass);	//������ ��� ����

	InitCommonControls();	//���� ��Ʈ�� �ʱ�ȭ �Լ�(ResisterClassEx�Լ� ���Ŀ� �����ؾ� �Ѵ�.)

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);


	//�޽��� ����
	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	HWND hModaless;

	/////// ���� ����, ���� ���� ���� ��ȭ���� ���� ///////
	OPENFILENAME ofn;
	TCHAR strFile[MAX_PATH] = L"";
	TCHAR strTemp[MAX_PATH];

	/////// ��Ʈ ���� ��ȭ���� ���� ///////
	static CHOOSEFONT font;
	static LOGFONT		setfont;
	HFONT	hFont, oldFont;
	TCHAR str[] = L"��Ʈ Test 1234";
	static bool fontflag = 0;

	/////// �÷� ���� ��ȭ���� ���� ///////
	static CHOOSECOLOR color;
	static COLORREF setColor;
	static COLORREF crCustColors[16];

	switch (iMessage)
	{
	case WM_CREATE:
		break;

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
			DialogBox(g_hInst, (LPWSTR)IDD_DIALOG, hWnd, DialogProc);	//hWnd : �θ� ������ �ڵ� ��
			break;
		case ID_MODALESS:
			hModaless = CreateDialog(g_hInst, (LPWSTR)IDD_DIALOG, hWnd, DialogProc);
			//ShowWindow �Լ��� ���ؼ��� ȭ�鿡 ��� ����
			ShowWindow(hModaless, SW_SHOW);
			break;
		case ID_FILEOPEN:
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = L"Text File(*.txt)\0*.txt;\0";	//Ȯ���� ���� ����(txt���� ���͸�)
			ofn.lpstrFile = strFile;
			ofn.nMaxFile = MAX_PATH;
			if (GetOpenFileName(&ofn) != 0)
			{
				wsprintf(strTemp, L"%s ������ �����߽��ϴ�.", ofn.lpstrFile);
				MessageBox(hWnd, strTemp, L"���� ����", MB_OK);
			}
			break;
		case ID_FILESAVE:
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = L"Text File(*.txt)\0*.txt;\0";
			ofn.lpstrFile = strFile;
			ofn.nMaxFile = MAX_PATH;
			if (GetSaveFileName(&ofn) != 0)		//���� ���� ���� ��ȭ����
			{
				wsprintf(strTemp, L"%s ������ �����߽��ϴ�.", ofn.lpstrFile);
				MessageBox(hWnd, strTemp, L"���� ����", MB_OK);
			}
			break;
		case ID_FONT:
			//��Ʈ ����
			font.lStructSize = sizeof(CHOOSEFONT);
			font.hwndOwner = hWnd;
			font.lpLogFont = &setfont;
			font.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;

			if (ChooseFont(&font))	//��Ʈ ��ȭ���� ���
			{
				fontflag = 1;
				InvalidateRgn(hWnd, NULL, TRUE);
			}
			break;
		case ID_COLOR:
			//�÷� ����
			setColor = RGB(0, 0, 0);
			color.hwndOwner = hWnd;
			color.lStructSize = sizeof(CHOOSECOLOR);
			color.rgbResult = setColor;
			color.Flags = CC_RGBINIT | CC_FULLOPEN;
			color.lpCustColors = crCustColors;
			if (ChooseColor(&color))
			{
				InvalidateRgn(hWnd, NULL, TRUE);
			}
			break;
		case ID_MENU_LISTCTRL:
			DialogBox(g_hInst, (LPWSTR)IDD_LISTCTRL, hWnd, ListCtrlProc);
			break;
		case ID_MENU_TREECTRL:
			DialogBox(g_hInst, (LPWSTR)IDD_TREECTRL, hWnd, TreeCtrlProc);
			break;
		case ID_MENU_ETCCTRL:
			DialogBox(g_hInst, (LPWSTR)IDD_TABCTRL, hWnd, EtcCtrlProc);
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

//�ڵ� ����
HWND hEdit;	//����Ʈ �ڽ�	
HWND hList;	//����Ʈ �ڽ�

BOOL CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR strEdit[128];	//����Ʈ �ڽ����� ���ڿ��� �޾ƿ� ��Ƴ��� ����

	switch (message)
	{
	case WM_INITDIALOG:		//��ȭ���� �ʱ�ȭ
		hEdit = GetDlgItem(hDlg, IDC_EDIT);	//����Ʈ �ڽ� �ڵ� ���
		hList = GetDlgItem(hDlg, IDC_LIST);	//����Ʈ �ڽ� �ڵ� ���
		break;
	case WM_COMMAND:		//��ȭ���� ���� �ڽ� ������ ��Ʈ�� ó��
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, TRUE);	//��ȭ���� ����
			break;
		case IDC_BTN_ADD:	//[�߰�]��ư Ŭ����
			GetWindowText(hEdit, strEdit, wcslen(strEdit));	//����Ʈ �ڽ����� �ؽ�Ʈ ������ strEdit�� ����
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)strEdit);	//strEdit ���ڿ��� ����Ʈ �ڽ��� �߰�
			break;
		}
	}
	return FALSE;
}

