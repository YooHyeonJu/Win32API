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


//대화 상자 기반에서 WinMain, WndProc을 추가하여, 윈도우 기반으로 변경함.(모달 방식)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{

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

	InitCommonControls();	//공용 컨트롤 초기화 함수(ResisterClassEx함수 이후에 삽입해야 한다.)

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
}


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

//핸들 생성
HWND hEdit;	//리스트 박스	
HWND hList;	//에디트 박스

BOOL CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR strEdit[128];	//에디트 박스에서 문자열을 받아와 담아놓을 버퍼

	switch (message)
	{
	case WM_INITDIALOG:		//대화상자 초기화
		hEdit = GetDlgItem(hDlg, IDC_EDIT);	//에디트 박스 핸들 얻기
		hList = GetDlgItem(hDlg, IDC_LIST);	//리스트 박스 핸들 얻기
		break;
	case WM_COMMAND:		//대화상자 위의 자식 윈도우 컨트롤 처리
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, TRUE);	//대화상자 종료
			break;
		case IDC_BTN_ADD:	//[추가]버튼 클릭시
			GetWindowText(hEdit, strEdit, wcslen(strEdit));	//에디트 박스에서 텍스트 가져와 strEdit에 복사
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)strEdit);	//strEdit 문자열을 리스트 박스에 추가
			break;
		}
	}
	return FALSE;
}

