#include <windows.h>
#include <tchar.h>
#include<commctrl.h>
//#include<CommCtrl.h>
#include"resource.h"

LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK CloseEnumProc(HWND, LPARAM);	//��� �ڽ� �����츦 �ϰ������� ����
BOOL CALLBACK ToolbarProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
const wchar_t* lpszClass = L"ToolBar";
const wchar_t* lpszChildClass = L"ChildView";

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

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd, hChildWnd;
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

	InitCommonControls();


	InitCommonControls();//���� ��Ʈ�� �ʱ�ȭ ��ƾ

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	hChildWnd = GetWindow(hWnd, GW_CHILD);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	while (GetMessage(&Message, 0, 0, 0)) {
		if (!TranslateMDISysAccel(hChildWnd, &Message))	//MDI��ݿ��� �ڽ� �������� �޽����� �ޱ� ���� �Լ�.
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	HDC hdc;
	PAINTSTRUCT ps;

	LPTBBUTTON ptb;
	static HWND toolbar, floatWnd,statusbar;

	LPNMHDR	pnmh;
	LPTOOLTIPTEXT lpToolTip;
	const wchar_t*pStrTooltipText = L"";

	HMENU hMenu;
	DWORD	style;

	int statusX, statusY;
	int gapY;
	RECT rect;

	TCHAR szTemp[20];
	UINT Idx;

	static HWND hClient;
	CLIENTCREATESTRUCT clientcreate;

	


	switch (iMessage)
	{
	case WM_CREATE:	//������ ������ ������ Ŭ���̾�Ʈ ������ ���� ������.
		ptb = tbb;
		toolbar = CreateToolbarEx(hWnd, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_BORDER | CCS_TOP | TBSTYLE_TOOLTIPS, 1, 1, g_hInst, IDB_BITMAP, ptb, 7, 16, 16, 16, 15, sizeof(TBBUTTON));

		//���� ����
		floatWnd = CreateDialog(g_hInst, (LPWSTR)IDD_FLOATTOOLBAR, hWnd, ToolbarProc);

		//���¹� ����
		statusbar = CreateStatusWindow(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CCS_BOTTOM, L"�غ� �Ǿ����ϴ�.", hWnd, 2);

		floatWnd = CreateDialog(g_hInst, (LPWSTR)IDD_FLOATTOOLBAR, hWnd, ToolbarProc);

		clientcreate.hWindowMenu = NULL;
		clientcreate.idFirstChild = 100;
		hClient = CreateWindow(L"MDICLIENT", NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)1, g_hInst, (LPWSTR)&clientcreate);
		//NULL : �޴� �ڵ��� ������ �����쿡 ���� ����ϹǷ� ���X
		//WS_CHILDREN : Ŭ���̾�Ʈ ������� ������ �����쿡 �پ ����.


		return 0;
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_FILENEW:
			MessageBox(hWnd, L"������ ���� ����ϴ�..", L"������", MB_OK);
			break;
		case ID_FILEOPEN:
			MessageBox(hWnd, L"������ ������ ���ϴ�.", L"���Ͽ���", MB_OK);
			break;
		case ID_FILESAVE:
			MessageBox(hWnd, L"������ �����մϴ�..", L"��������", MB_OK);
			break;
		case ID_FILEPRINT:
			MessageBox(hWnd, L"������ ����մϴ�.", L"����ϱ�", MB_OK);
			break;
		case ID_EDITCUT:
			MessageBox(hWnd, L"������ �߶���ϴ�.", L"�߶󳻱�", MB_OK);
			break;
		case ID_EDITCOPY:
			MessageBox(hWnd, L"������ �����մϴ�.", L"�����ϱ�", MB_OK);
			break;
		case ID_EDITPASTE:
			MessageBox(hWnd, L"������ �ٿ��ֱ��մϴ�.", L"�ٿ��ֱ�", MB_OK);
			break;
		case ID_HELPEXE:
			MessageBox(hWnd, L"������ ����մϴ�", L"����", MB_OK);
			break;

		case ID_FLOAT:
			hMenu = GetMenu(hWnd);
			EnableMenuItem(hMenu, ID_FLOAT, MF_BYCOMMAND | MF_GRAYED);//��Ȱ��
			EnableMenuItem(hMenu, ID_DOCKING, MF_BYCOMMAND | MF_ENABLED);//Ȱ��

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
		
	case WM_NOTIFY:
		pnmh = (LPNMHDR)lParam;
		if (pnmh->code == TTN_NEEDTEXT)
		{	//�ʿ��� ���ڿ��� ����
			lpToolTip = (LPTOOLTIPTEXT)lParam;
			//�� �׸� ���� ǳ�� ������ �����Ѵ�.
			switch (lpToolTip->hdr.idFrom)\
			{
			case ID_FILENEW:
				pStrTooltipText = L"������";
				break;
			case ID_FILEOPEN:
				pStrTooltipText = L"���Ͽ���";
				break;
			case ID_FILESAVE:
				pStrTooltipText = L"��������";
				break;
			case ID_EDITCUT:
				pStrTooltipText = L"�߶󳻱�";
				break;
			case ID_EDITCOPY:
				pStrTooltipText = L"����";
				break;
			case ID_EDITPASTE:
				pStrTooltipText = L"�ٿ��ֱ�";
				break;
			case ID_FILEPRINT:
				pStrTooltipText = L"�μ�";
				break;
			case ID_HELPEXE:
				pStrTooltipText = L"����";
				break;
			}
			wcscpy(lpToolTip->szText, pStrTooltipText);
			//���¹ٿ� ���ڿ� ���ҽ��� �����͸� ����Ѵ�.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
			Idx = lpToolTip->hdr.idFrom;
			LoadString(g_hInst, Idx, szTemp, 80);
			SetWindowText(statusbar, szTemp);
		}
		return 0;

	case WM_SIZE:
		statusX = LOWORD(lParam);
		statusY = HIWORD(lParam);

		GetWindowRect(statusbar, &rect);
		gapY = rect.bottom - rect.top;
		MoveWindow(statusbar, 0, statusY - gapY, statusX, gapY, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefFrameProc(hWnd, hClient, iMessage, wParam, lParam));
}

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	return DefMDIChildProc(hWnd, iMessage, wParam, lParam);
}


BOOL CALLBACK CloseEnumProc(HWND hWnd, LPARAM lParam)
{
	if (GetWindow(hWnd, GW_OWNER))
		return 1;

	SendMessage(GetParent(hWnd), WM_MDIRESTORE, (WPARAM)hWnd, 0);

	if (!SendMessage(hWnd, WM_QUERYENDSESSION, 0, 0))
		return 1;
	SendMessage(GetParent(hWnd), WM_MDIDESTROY, (WPARAM)hWnd, 0);
	return 1;
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
	}

	return FALSE;
}