#include <windows.h>
#include <tchar.h>
#include "resource.h"
//#include <commctrl.h>
//#pragma comment(lib,"commctl32.lib")
//#pragma comment(lib, "comctl32.lib")

BOOL CALLBACK		DialogProc(HWND, UINT, WPARAM, LPARAM);
//BOOL CALLBACK		ListCtrlProc(HWND, UINT, WPARAM, LPARAM);
//BOOL CALLBACK		TreeCtrlProc(HWND, UINT, WPARAM, LPARAM);
//BOOL CALLBACK		EtcCtrlProc(HWND, UINT, WPARAM, LPARAM);

//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//HINSTANCE g_hInst;
//const wchar_t* lpszClass = L"Dialog";


//��ȭ ���� ��ݿ��� WinMain, WndProc�� �߰��Ͽ�, ������ ������� ������.(��� ���)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	/*
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

	//InitCommonControls();	//���� ��Ʈ�� �ʱ�ȭ �Լ�(ResisterClassEx�Լ� ���Ŀ� �����ؾ� �Ѵ�.)

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
	*/

	DialogBox(hInstance, (LPWSTR)IDD_DIALOG, HWND_DESKTOP, DialogProc);
}

/*
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
		//
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
		//
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
*/

//�ڵ� ����
HWND hEdit;	//����Ʈ �ڽ�	
HWND hList;	//����Ʈ �ڽ�

BOOL CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	TCHAR strEdit[128];	//����Ʈ �ڽ����� ���ڿ��� �޾ƿ� ��Ƴ��� ����

	switch (message)
	{
	case WM_INITDIALOG:		//��ȭ���� �ʱ�ȭ
		//hEdit = GetDlgItem(hDlg, IDC_EDIT);	//����Ʈ �ڽ� �ڵ� ���
		//hList = GetDlgItem(hDlg, IDC_LIST);	//����Ʈ �ڽ� �ڵ� ���
		break;
	case WM_COMMAND:		//��ȭ���� ���� �ڽ� ������ ��Ʈ�� ó��

		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, TRUE);	//��ȭ���� ����
			break;
			/*
		case IDC_BTN_ADD:	//[�߰�]��ư Ŭ����
			GetWindowText(hEdit, strEdit, wcslen(strEdit));	//����Ʈ �ڽ����� �ؽ�Ʈ ������ strEdit�� ����
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)strEdit);	//strEdit ���ڿ��� ����Ʈ �ڽ��� �߰�
			break;
			*/
		}
		
	}
	
	return FALSE;
}

/*
TCHAR *Columnlist[3] = { (TCHAR*)"�̸�",(TCHAR*)"�ּ�",(TCHAR*)"�޴���" };//{ _T(L"�̸�"), _T(L"�ּ�"), _T(L"�޴���") };
int nWidth[3] = { 100, 150, 150 };
BOOL CALLBACK ListCtrlProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	LVCOLUMN column;
	LVITEM listItem;
	static HWND hListCtrl;
	static HWND hType;
	LPNMHDR	pnmh;
	LPNM_LISTVIEW pInfo;
	TCHAR szTemp[MAX_PATH];
	DWORD style;
	int nGetSel;
	//�̹��� ����Ʈ ���
	static HIMAGELIST  hImgSm, hImgLg;

	switch (message)
	{
	case WM_INITDIALOG:
		hListCtrl = GetDlgItem(hDlg, IDC_LISTCTRL);
		hType = GetDlgItem(hDlg, IDC_COMBO_TYPE);
		hImgSm = ImageList_LoadBitmap(g_hInst, MAKEINTRESOURCE(ID_SEX16),
			16, 1, RGB(255, 0, 255));
		hImgLg = ImageList_LoadBitmap(g_hInst, MAKEINTRESOURCE(ID_SEX32),
			32, 1, RGB(255, 255, 255));
		SendMessage(hListCtrl, LVM_SETIMAGELIST, (WPARAM)LVSIL_SMALL, (LPARAM)hImgSm);
		SendMessage(hListCtrl, LVM_SETIMAGELIST, (WPARAM)LVSIL_NORMAL, (LPARAM)hImgLg);


		for (int i = 0; i < 3; i++)
		{
			column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
			column.fmt = LVCFMT_LEFT;			//�÷� ���� �÷���
			column.cx = nWidth[i];				//�÷� ���� ũ��
			column.pszText = Columnlist[i];		//�÷� �ؽ�Ʈ
			column.iSubItem = 0;				//�÷� ��ȣ
			ListView_InsertColumn(hListCtrl, i, &column);	//����Ʈ �÷��� ����� ���� �Լ�.
		}

		listItem.mask = LVIF_TEXT | LVIF_IMAGE;	//�ؽ�Ʈ�� �̹��� ǥ��
		listItem.state = 0;
		listItem.stateMask = 0;
		listItem.iImage = 0;
		listItem.iSubItem = 0;
		listItem.iItem = 0;		//1��° ������
		listItem.pszText = (LPWSTR)L"��â��";
		ListView_InsertItem(hListCtrl, &listItem);

		//���� �������� ������ �� : ListView_SetItemText �Լ� ���
		ListView_SetItemText(hListCtrl, 0, 1, (LPWSTR)L"������ ���뱸");
		ListView_SetItemText(hListCtrl, 0, 2, (LPWSTR)L"010-4477-1234");

		listItem.mask = LVIF_TEXT | LVIF_IMAGE;
		listItem.state = 0;
		listItem.stateMask = 0;
		listItem.iImage = 0;
		listItem.iSubItem = 0;
		listItem.iItem = 1;
		listItem.pszText = (LPWSTR)L"���ּ�";
		ListView_InsertItem(hListCtrl, &listItem);

		ListView_SetItemText(hListCtrl, 1, 1, (LPWSTR)L"������ ���뱸");
		ListView_SetItemText(hListCtrl, 1, 2, (LPWSTR)L"019-672-1234");

		listItem.mask = LVIF_TEXT | LVIF_IMAGE;
		listItem.state = 0;
		listItem.stateMask = 0;
		listItem.iImage = 1;
		listItem.iSubItem = 0;
		listItem.iItem = 2;
		listItem.pszText = (LPWSTR)L"����ȭ";
		ListView_InsertItem(hListCtrl, &listItem);

		ListView_SetItemText(hListCtrl, 2, 1, (LPWSTR)L"������ ���뱸");
		ListView_SetItemText(hListCtrl, 2, 2, (LPWSTR)L"010-2773-1234");

		SendMessage(hType, CB_ADDSTRING, 0, (LPARAM)L"Icon");
		SendMessage(hType, CB_ADDSTRING, 1, (LPARAM)L"Small Icon");
		SendMessage(hType, CB_ADDSTRING, 2, (LPARAM)L"List");
		SendMessage(hType, CB_ADDSTRING, 3, (LPARAM)L"Report");

		SendMessage(hType, CB_SETCURSEL, 3, 0);	//�޺��ڽ� ����Ʈ �� �ʱ� ���� ������ ����
		break;
	case WM_NOTIFY:		//��Ʈ�ѿ� �̺�Ʈ �߻�
		if (wParam == IDC_LISTCTRL)
		{
			pnmh = (LPNMHDR)lParam;
			switch (pnmh->code)
			{
			case LVN_ITEMCHANGED:	//ITEM ������ ��ȭ
				pInfo = (LPNM_LISTVIEW)lParam;
				ListView_GetItemText(hListCtrl, pInfo->iItem, pInfo->iSubItem, szTemp, MAX_PATH);
				SetWindowText(GetDlgItem(hDlg, IDC_ITEMTEXT), szTemp);	//���
				break;
			}
		}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_COMBO_TYPE:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:		//����Ʈ �������� ������ ����� ��
				nGetSel = SendMessage(hType, CB_GETCURSEL, 0, 0);
				//SendMessage(hType, CB_GETLBTEXT, nGetSel, (LPARAM)szTemp);
				switch (nGetSel)
				{
				case 0:
					style = GetWindowLong(hListCtrl, GWL_STYLE);	//������ ��Ÿ�� ����.
					style &= ~(LVS_TYPEMASK);
					style = style | LVS_ICON;
					SetWindowLong(hListCtrl, GWL_STYLE, style);	//������ ��Ÿ�� ����
					break;
				case 1:
					style = GetWindowLong(hListCtrl, GWL_STYLE);
					style &= ~(LVS_TYPEMASK);
					style = style | LVS_SMALLICON;
					SetWindowLong(hListCtrl, GWL_STYLE, style);
					break;
				case 2:
					style = GetWindowLong(hListCtrl, GWL_STYLE);
					style &= ~(LVS_TYPEMASK);
					style = style | LVS_LIST;
					SetWindowLong(hListCtrl, GWL_STYLE, style);
					break;
				case 3:
					style = GetWindowLong(hListCtrl, GWL_STYLE);
					style &= ~(LVS_TYPEMASK);
					style = style | LVS_REPORT;
					SetWindowLong(hListCtrl, GWL_STYLE, style);
					break;
				}
				break;
			}
			break;
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg, TRUE);
			break;
		}
	case WM_DESTROY:
		//		DestroyWindow(hDlg);
		return FALSE;
	}
	return FALSE;
	//return (DefWindowProc(hDlg, message, wParam, lParam));
}
*/


/*
HTREEITEM					itemlist[7];
TV_INSERTSTRUCT		tvstruct[7];
TCHAR *pItemlist[7] = { (TCHAR*)("Visual C++"), (TCHAR*)("ATL"), (TCHAR*)("CLR"),(TCHAR*)("�Ϲ�"), (TCHAR*)("MFC"), (TCHAR*)("�ٸ� ���"), (TCHAR*)("Visual C#") };
BOOL CALLBACK TreeCtrlProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hTreeCtrl;
	HTREEITEM	rootItem;
	//static HIMAGELIST  hTreeImage;

	POINT point;
	TV_HITTESTINFO			hitinfo;
	HTREEITEM					hItem;
	LPCWSTR						ItemText;
	LPNMHDR						pnmh;

	switch (message)
	{
	case WM_INITDIALOG:
		hTreeCtrl = GetDlgItem(hDlg, IDC_TREECTRL);
		//hTreeImage=ImageList_LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SEX16), 
		//	16, 1, RGB(255,0,255));
		//SendMessage(hTreeCtrl, TVM_SETIMAGELIST, (WPARAM)0, (LPARAM)hTreeImage);

		//TV_INSERTSTRUCT		tvstruct;
		for (int i = 0; i< 7; i++)
		{
			if (pItemlist[i] == L"Visual C++" || pItemlist[i] == L"�ٸ� ���")
			{
				tvstruct[i].hParent = NULL;
				tvstruct[i].hInsertAfter = TVI_LAST;
				tvstruct[i].item.pszText = pItemlist[i];
				tvstruct[i].item.mask = TVIF_TEXT;
				rootItem = TreeView_InsertItem(hTreeCtrl, &tvstruct[i]);
				itemlist[i] = rootItem;
			}
			else
			{
				tvstruct[i].hParent = rootItem;
				tvstruct[i].hInsertAfter = TVI_LAST;
				tvstruct[i].item.pszText = pItemlist[i];
				tvstruct[i].item.mask = TVIF_TEXT;
				hItem = TreeView_InsertItem(hTreeCtrl, &tvstruct[i]);
				itemlist[i] = hItem;
			}
		}

		//tvstruct.hParent = NULL;
		//tvstruct.hInsertAfter = TVI_LAST;
		//tvstruct.item.pszText = L"Visual C++";
		//tvstruct.item.mask = TVIF_TEXT | TVIF_IMAGE;
		//rootItem = TreeView_InsertItem(hTreeCtrl, &tvstruct);

		//tvstruct.hParent = rootItem;
		//tvstruct.hInsertAfter = TVI_LAST;
		//tvstruct.item.pszText = L"ATL";
		//tvstruct.item.mask = TVIF_TEXT| TVIF_IMAGE;
		//TreeView_InsertItem(hTreeCtrl, &tvstruct);
		//
		//tvstruct.hParent = rootItem;
		//tvstruct.hInsertAfter = TVI_LAST;
		//tvstruct.item.pszText = L"CLR";
		//tvstruct.item.mask = TVIF_TEXT;
		//TreeView_InsertItem(hTreeCtrl, &tvstruct);

		//tvstruct.hParent = rootItem;
		//tvstruct.hInsertAfter = TVI_LAST;
		//tvstruct.item.pszText = L"�Ϲ�";
		//tvstruct.item.mask = TVIF_TEXT;
		//TreeView_InsertItem(hTreeCtrl, &tvstruct);

		//tvstruct.hParent = rootItem;
		//tvstruct.hInsertAfter = TVI_LAST;
		//tvstruct.item.pszText = L"MFC";
		//tvstruct.item.mask = TVIF_TEXT;
		//TreeView_InsertItem(hTreeCtrl, &tvstruct);

		//tvstruct.hParent = NULL;
		//tvstruct.hInsertAfter = TVI_LAST;
		//tvstruct.item.pszText = L"�ٸ� ���";
		//tvstruct.item.mask = TVIF_TEXT;
		//rootItem = TreeView_InsertItem(hTreeCtrl, &tvstruct);

		//tvstruct.hParent = rootItem;
		//tvstruct.hInsertAfter = TVI_LAST;
		//tvstruct.item.pszText = L"Visual C#";
		//tvstruct.item.mask = TVIF_TEXT;
		//TreeView_InsertItem(hTreeCtrl, &tvstruct);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg, TRUE);
			break;
		}

	case WM_NOTIFY:
		if (wParam == IDC_TREECTRL)
		{
			pnmh = (LPNMHDR)lParam;
			switch (pnmh->code)
			{
			case TVN_SELCHANGED:
				GetCursorPos(&point);
				ScreenToClient(hTreeCtrl, &point);
				hitinfo.pt = point;
				hitinfo.flags = TVHT_ONITEM;
				hItem = TreeView_HitTest(hTreeCtrl, &hitinfo);

				for (int i = 0; i< 7; i++)
					if (hItem == itemlist[i])
						ItemText = tvstruct[i].item.pszText;
				SetWindowText(GetDlgItem(hDlg, IDC_ITEMTEXT), ItemText);
				break;
			}
		}
	}
	return FALSE;
}

*/

/*
BOOL CALLBACK EtcCtrlProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hTabCtrl;
	static HWND hProgress;
	static HWND hBtnStart;
	static HWND hSlider;
	static HWND hSpin;
	static HWND hSliderEdit;
	static HWND hSpinEdit;
	static int pgpos = 0;
	static int sdpos = 0;
	TCHAR szTemp[10];

	//�� ��Ʈ�� 
	LPNMHDR	pnmh;
	int nTabNum;

	switch (message)
	{
	case WM_INITDIALOG:
		hTabCtrl = GetDlgItem(hDlg, IDC_TABCTRL);
		hProgress = GetDlgItem(hDlg, IDC_PROGRESS);
		hBtnStart = GetDlgItem(hDlg, IDC_BUTTON_START);
		hSlider = GetDlgItem(hDlg, IDC_SLIDER);
		hSliderEdit = GetDlgItem(hDlg, IDC_EDIT_SLIDER);
		hSpin = GetDlgItem(hDlg, IDC_SPIN);
		hSpinEdit = GetDlgItem(hDlg, IDC_EDIT_SPIN);


		TC_ITEM		tabItem;
		tabItem.mask = TCIF_TEXT | TCIF_IMAGE;
		tabItem.pszText = (LPWSTR)L"���α׷�����";
		TabCtrl_InsertItem(hTabCtrl, 0, &tabItem);

		tabItem.pszText = (LPWSTR)L"�����̴�";
		TabCtrl_InsertItem(hTabCtrl, 1, &tabItem);

		tabItem.pszText = (LPWSTR)L"����";
		TabCtrl_InsertItem(hTabCtrl, 2, &tabItem);

		ShowWindow(hProgress, SW_SHOW);
		ShowWindow(hBtnStart, SW_SHOW);
		ShowWindow(hSlider, SW_HIDE);
		ShowWindow(hSliderEdit, SW_HIDE);
		ShowWindow(hSpin, SW_HIDE);
		ShowWindow(hSpinEdit, SW_HIDE);

		//���α׷������� ���� ���� �� �ʱ�ȭ
		SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
		SendMessage(hProgress, PBM_SETPOS, (WPARAM)10, (LPARAM)NULL);

		//�����̴� ���� ����
		SendMessage(hSlider, TBM_SETRANGE, 0, MAKELPARAM(0, 10));

		//���� ���� ����
		SendMessage(hSpin, UDM_SETRANGE, 0, MAKELPARAM(0, 10));

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_START:
			if (HIWORD(wParam) == BN_CLICKED)
				SetTimer(hDlg, 1, 500, NULL);
			break;
		}
		break;
	case WM_TIMER:
		if (pgpos  >  100)
			pgpos = 0;
		pgpos += 10;

		SendMessage(hProgress, PBM_SETPOS, (WPARAM)pgpos, (LPARAM)NULL);
		break;

	case WM_NOTIFY:
		if (wParam == IDC_TABCTRL)
		{
			pnmh = (LPNMHDR)lParam;
			switch (pnmh->code)
			{
			case TCN_SELCHANGE:
				nTabNum = TabCtrl_GetCurFocus(hTabCtrl);
				if (nTabNum == 0)
				{
					ShowWindow(hProgress, SW_SHOW);
					ShowWindow(hBtnStart, SW_SHOW);
					ShowWindow(hSlider, SW_HIDE);
					ShowWindow(hSliderEdit, SW_HIDE);
					ShowWindow(hSpin, SW_HIDE);
					ShowWindow(hSpinEdit, SW_HIDE);
					break;
				}
				else if (nTabNum == 1)
				{
					ShowWindow(hProgress, SW_HIDE);
					ShowWindow(hBtnStart, SW_HIDE);
					ShowWindow(hSlider, SW_SHOW);
					ShowWindow(hSliderEdit, SW_SHOW);
					ShowWindow(hSpin, SW_HIDE);
					ShowWindow(hSpinEdit, SW_HIDE);
					break;
				}
				else if (nTabNum == 2)
				{
					ShowWindow(hProgress, SW_HIDE);
					ShowWindow(hBtnStart, SW_HIDE);
					ShowWindow(hSlider, SW_HIDE);
					ShowWindow(hSliderEdit, SW_HIDE);
					ShowWindow(hSpin, SW_SHOW);
					ShowWindow(hSpinEdit, SW_SHOW);
					break;
				}
			}
		}

		if (wParam == IDC_SLIDER)
		{
			sdpos = SendMessage(hSlider, TBM_GETPOS, 0, 0);
			wsprintf(szTemp, L"%d", sdpos);
			SetWindowText(hSliderEdit, szTemp);
		}

		return FALSE;

	case WM_CLOSE:
		KillTimer(hDlg, 1);
		EndDialog(hDlg, TRUE);
		return FALSE;

	}
	return FALSE;
}
*/