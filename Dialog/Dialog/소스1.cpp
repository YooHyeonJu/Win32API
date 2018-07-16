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


//대화 상자 기반에서 WinMain, WndProc을 추가하여, 윈도우 기반으로 변경함.(모달 방식)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	/*
	///기본 윈도우 생성
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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);	//기본 윈도우 생성 시 메뉴 사용
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&WndClass);	//윈도우 등록 과정

	//InitCommonControls();	//공용 컨트롤 초기화 함수(ResisterClassEx함수 이후에 삽입해야 한다.)

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);


	//메시지 루프
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

	/////// 파일 열기, 파일 저장 공용 대화상자 변수 ///////
	OPENFILENAME ofn;
	TCHAR strFile[MAX_PATH] = L"";
	TCHAR strTemp[MAX_PATH];

	/////// 폰트 공용 대화상자 변수 ///////
	static CHOOSEFONT font;
	static LOGFONT		setfont;
	HFONT	hFont, oldFont;
	TCHAR str[] = L"폰트 Test 1234";
	static bool fontflag = 0;

	/////// 컬러 공용 대화상자 변수 ///////
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
			DialogBox(g_hInst, (LPWSTR)IDD_DIALOG, hWnd, DialogProc);	//hWnd : 부모 윈도우 핸들 값
			break;
		case ID_MODALESS:
			hModaless = CreateDialog(g_hInst, (LPWSTR)IDD_DIALOG, hWnd, DialogProc);
			//ShowWindow 함수를 통해서만 화면에 출력 가능
			ShowWindow(hModaless, SW_SHOW);
			break;
		case ID_FILEOPEN:
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = L"Text File(*.txt)\0*.txt;\0";	//확장자 필터 설정(txt만을 필터링)
			ofn.lpstrFile = strFile;
			ofn.nMaxFile = MAX_PATH;
			if (GetOpenFileName(&ofn) != 0)
			{
				wsprintf(strTemp, L"%s 파일을 선택했습니다.", ofn.lpstrFile);
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
			if (GetSaveFileName(&ofn) != 0)		//파일 저장 공용 대화상자
			{
				wsprintf(strTemp, L"%s 파일을 선택했습니다.", ofn.lpstrFile);
				MessageBox(hWnd, strTemp, L"파일 저장", MB_OK);
			}
			break;
		case ID_FONT:
			//폰트 설정
			font.lStructSize = sizeof(CHOOSEFONT);
			font.hwndOwner = hWnd;
			font.lpLogFont = &setfont;
			font.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;

			if (ChooseFont(&font))	//폰트 대화상자 출력
			{
				fontflag = 1;
				InvalidateRgn(hWnd, NULL, TRUE);
			}
			break;
		case ID_COLOR:
			//컬러 설정
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

//핸들 생성
HWND hEdit;	//리스트 박스	
HWND hList;	//에디트 박스

BOOL CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	TCHAR strEdit[128];	//에디트 박스에서 문자열을 받아와 담아놓을 버퍼

	switch (message)
	{
	case WM_INITDIALOG:		//대화상자 초기화
		//hEdit = GetDlgItem(hDlg, IDC_EDIT);	//에디트 박스 핸들 얻기
		//hList = GetDlgItem(hDlg, IDC_LIST);	//리스트 박스 핸들 얻기
		break;
	case WM_COMMAND:		//대화상자 위의 자식 윈도우 컨트롤 처리

		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, TRUE);	//대화상자 종료
			break;
			/*
		case IDC_BTN_ADD:	//[추가]버튼 클릭시
			GetWindowText(hEdit, strEdit, wcslen(strEdit));	//에디트 박스에서 텍스트 가져와 strEdit에 복사
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)strEdit);	//strEdit 문자열을 리스트 박스에 추가
			break;
			*/
		}
		
	}
	
	return FALSE;
}

/*
TCHAR *Columnlist[3] = { (TCHAR*)"이름",(TCHAR*)"주소",(TCHAR*)"휴대폰" };//{ _T(L"이름"), _T(L"주소"), _T(L"휴대폰") };
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
	//이미지 리스트 등록
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
			column.fmt = LVCFMT_LEFT;			//컬럼 정렬 플래그
			column.cx = nWidth[i];				//컬럼 가로 크기
			column.pszText = Columnlist[i];		//컬럼 텍스트
			column.iSubItem = 0;				//컬럼 번호
			ListView_InsertColumn(hListCtrl, i, &column);	//리스트 컬럼을 만들기 위한 함수.
		}

		listItem.mask = LVIF_TEXT | LVIF_IMAGE;	//텍스트와 이미지 표현
		listItem.state = 0;
		listItem.stateMask = 0;
		listItem.iImage = 0;
		listItem.iSubItem = 0;
		listItem.iItem = 0;		//1번째 아이템
		listItem.pszText = (LPWSTR)L"이창현";
		ListView_InsertItem(hListCtrl, &listItem);

		//서브 아이템을 설정할 때 : ListView_SetItemText 함수 사용
		ListView_SetItemText(hListCtrl, 0, 1, (LPWSTR)L"수원시 영통구");
		ListView_SetItemText(hListCtrl, 0, 2, (LPWSTR)L"010-4477-1234");

		listItem.mask = LVIF_TEXT | LVIF_IMAGE;
		listItem.state = 0;
		listItem.stateMask = 0;
		listItem.iImage = 0;
		listItem.iSubItem = 0;
		listItem.iItem = 1;
		listItem.pszText = (LPWSTR)L"이주성";
		ListView_InsertItem(hListCtrl, &listItem);

		ListView_SetItemText(hListCtrl, 1, 1, (LPWSTR)L"수원시 영통구");
		ListView_SetItemText(hListCtrl, 1, 2, (LPWSTR)L"019-672-1234");

		listItem.mask = LVIF_TEXT | LVIF_IMAGE;
		listItem.state = 0;
		listItem.stateMask = 0;
		listItem.iImage = 1;
		listItem.iSubItem = 0;
		listItem.iItem = 2;
		listItem.pszText = (LPWSTR)L"조경화";
		ListView_InsertItem(hListCtrl, &listItem);

		ListView_SetItemText(hListCtrl, 2, 1, (LPWSTR)L"수원시 영통구");
		ListView_SetItemText(hListCtrl, 2, 2, (LPWSTR)L"010-2773-1234");

		SendMessage(hType, CB_ADDSTRING, 0, (LPARAM)L"Icon");
		SendMessage(hType, CB_ADDSTRING, 1, (LPARAM)L"Small Icon");
		SendMessage(hType, CB_ADDSTRING, 2, (LPARAM)L"List");
		SendMessage(hType, CB_ADDSTRING, 3, (LPARAM)L"Report");

		SendMessage(hType, CB_SETCURSEL, 3, 0);	//콤보박스 리스트 중 초기 선택 아이템 설정
		break;
	case WM_NOTIFY:		//컨트롤에 이벤트 발생
		if (wParam == IDC_LISTCTRL)
		{
			pnmh = (LPNMHDR)lParam;
			switch (pnmh->code)
			{
			case LVN_ITEMCHANGED:	//ITEM 선택의 변화
				pInfo = (LPNM_LISTVIEW)lParam;
				ListView_GetItemText(hListCtrl, pInfo->iItem, pInfo->iSubItem, szTemp, MAX_PATH);
				SetWindowText(GetDlgItem(hDlg, IDC_ITEMTEXT), szTemp);	//출력
				break;
			}
		}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_COMBO_TYPE:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:		//리스트 아이템의 선택이 변경될 때
				nGetSel = SendMessage(hType, CB_GETCURSEL, 0, 0);
				//SendMessage(hType, CB_GETLBTEXT, nGetSel, (LPARAM)szTemp);
				switch (nGetSel)
				{
				case 0:
					style = GetWindowLong(hListCtrl, GWL_STYLE);	//윈도우 스타일 얻어옴.
					style &= ~(LVS_TYPEMASK);
					style = style | LVS_ICON;
					SetWindowLong(hListCtrl, GWL_STYLE, style);	//윈도우 스타일 변경
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
TCHAR *pItemlist[7] = { (TCHAR*)("Visual C++"), (TCHAR*)("ATL"), (TCHAR*)("CLR"),(TCHAR*)("일반"), (TCHAR*)("MFC"), (TCHAR*)("다른 언어"), (TCHAR*)("Visual C#") };
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
			if (pItemlist[i] == L"Visual C++" || pItemlist[i] == L"다른 언어")
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
		//tvstruct.item.pszText = L"일반";
		//tvstruct.item.mask = TVIF_TEXT;
		//TreeView_InsertItem(hTreeCtrl, &tvstruct);

		//tvstruct.hParent = rootItem;
		//tvstruct.hInsertAfter = TVI_LAST;
		//tvstruct.item.pszText = L"MFC";
		//tvstruct.item.mask = TVIF_TEXT;
		//TreeView_InsertItem(hTreeCtrl, &tvstruct);

		//tvstruct.hParent = NULL;
		//tvstruct.hInsertAfter = TVI_LAST;
		//tvstruct.item.pszText = L"다른 언어";
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

	//탭 컨트롤 
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
		tabItem.pszText = (LPWSTR)L"프로그래스바";
		TabCtrl_InsertItem(hTabCtrl, 0, &tabItem);

		tabItem.pszText = (LPWSTR)L"슬라이더";
		TabCtrl_InsertItem(hTabCtrl, 1, &tabItem);

		tabItem.pszText = (LPWSTR)L"스핀";
		TabCtrl_InsertItem(hTabCtrl, 2, &tabItem);

		ShowWindow(hProgress, SW_SHOW);
		ShowWindow(hBtnStart, SW_SHOW);
		ShowWindow(hSlider, SW_HIDE);
		ShowWindow(hSliderEdit, SW_HIDE);
		ShowWindow(hSpin, SW_HIDE);
		ShowWindow(hSpinEdit, SW_HIDE);

		//프로그래스바의 구간 설정 및 초기화
		SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
		SendMessage(hProgress, PBM_SETPOS, (WPARAM)10, (LPARAM)NULL);

		//슬라이더 구간 설정
		SendMessage(hSlider, TBM_SETRANGE, 0, MAKELPARAM(0, 10));

		//스핀 구간 설정
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