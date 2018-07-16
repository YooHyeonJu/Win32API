#include <windows.h>
#include <tchar.h>

#define ID_CHK_RETANGLE 300
#define ID_CHK_ELLIPSE 301

#define ID_RADIO1 401
#define ID_RADIO2 402
#define ID_RADIO3 403
#define ID_RADIO4 404
#define ID_RADIO5 405

#define ID_EDIT 501
#define ID_LIST 601
#define ID_COMBO 701
#define ID_SCROLL 801


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"ChildWindow";

HWND g_hStatic1;
HWND g_hStatic2;
HWND g_hEdit;	//����Ʈ �ڽ� �ڵ�
HWND g_hList;	//����Ʈ �ڽ�
HWND g_hCombo;
HWND g_hScroll;

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
		200,50,600,600,
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
	HBRUSH MyBrush, OldBrush;
	static short nGraph = 0;
	static short nColor = 0;

	static HWND hChkRectangle, hChkEllipse;
	static BOOL bEllipse = FALSE;
	static BOOL bRectangle = FALSE;

	TCHAR strEdit[28];
	TCHAR strList[28];
	TCHAR listItem[][30] = { L"C++���α׷���",L"Win32 ���α׷���",L"MFC �ý��� ���α׷���",L"�ڹ� ���α׷���" };
	int nGetSel;
	
	static int nScrollMove = 0;
	

	switch (iMessage)
	{
	case WM_CREATE:
		CreateWindow(L"button", L"��ư1", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 20, 100, 25, hWnd, (HMENU)100, g_hInst, NULL);
		CreateWindow(L"button", L"��ư2", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 60, 100, 25, hWnd, (HMENU)200, g_hInst, NULL);
		hChkRectangle = CreateWindow(L"button", L"�簢��", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 20, 120, 80, 25, hWnd, (HMENU)ID_CHK_RETANGLE, g_hInst, NULL);
		hChkEllipse = CreateWindow(L"button", L"Ÿ��", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 20, 160, 80, 25, hWnd, (HMENU)ID_CHK_ELLIPSE, g_hInst, NULL);
		CreateWindow(L"button", L"����", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 5, 220, 100, 100, hWnd, (HMENU)0, g_hInst, NULL);
		CreateWindow(L"button", L"����", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 140, 220, 100, 150, hWnd, (HMENU)1, g_hInst, NULL);
		CreateWindow(L"button", L"�簢��", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 20, 240, 80, 25, hWnd, (HMENU)ID_RADIO1, g_hInst, NULL);
		CreateWindow(L"button", L"Ÿ��", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 280, 80, 25, hWnd, (HMENU)ID_RADIO2, g_hInst, NULL);
		CreateWindow(L"button", L"����", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 150, 240, 80, 25, hWnd, (HMENU)ID_RADIO3, g_hInst, NULL);
		CreateWindow(L"button", L"�ʷ�", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 150, 280, 80, 25, hWnd, (HMENU)ID_RADIO4, g_hInst, NULL);
		CreateWindow(L"button", L"�Ķ�", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 150, 320, 80, 25, hWnd, (HMENU)ID_RADIO5, g_hInst, NULL);

		CheckRadioButton(hWnd, ID_RADIO1, ID_RADIO2, ID_RADIO1);
		CheckRadioButton(hWnd, ID_RADIO3, ID_RADIO5, ID_RADIO3);

		g_hStatic1 = CreateWindow(L"static", L"������ ���α׷���", WS_CHILD | WS_VISIBLE | SS_CENTER, 280, 20, 200, 25, hWnd, (HMENU)-1, g_hInst, NULL);
		g_hStatic2 = CreateWindow(L"static", L"����", WS_CHILD | WS_VISIBLE, 280, 60, 200, 80, hWnd, (HMENU)-1, g_hInst, NULL);
		//ID�� -1�� ������ ���� ���� ������� ���� �Ұ��� �ϱ� ������, ID�� �����ϴ� ���� �ǹ̾��� �����̴�.
		
		g_hEdit = CreateWindow(L"edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL | ES_MULTILINE,280,160,200,80,hWnd,(HMENU)ID_EDIT,g_hInst,NULL);
		g_hList = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD, 280, 260, 200, 80, hWnd, (HMENU)ID_LIST, g_hInst, NULL);
		for (int i = 0; i < 4; i++) 	SendMessage(g_hList, LB_ADDSTRING, 0, (LPARAM)listItem[i]);

		g_hCombo = CreateWindow(L"combobox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN, 280, 360, 200, 100, hWnd, (HMENU)ID_COMBO, g_hInst, NULL);
		for (int i = 0; i < 4; i++)		SendMessage(g_hCombo, CB_ADDSTRING, 0, (LPARAM)listItem[i]);

		g_hScroll = CreateWindow(L"scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, 280, 420, 200, 20, hWnd, (HMENU)ID_SCROLL, g_hInst, NULL);
		
		SetScrollRange(g_hScroll, SB_CTL, 0, 255, TRUE);	//�ʱ� ���� ����(0~255)
		SetScrollPos(g_hScroll, SB_CTL, 0, TRUE);			//�ʱ� ��ġ ����
		return 0; 
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			case 100:
				MessageBox(hWnd, L"��ư1�� �̺�Ʈ�� �߻����׽��ϴ�.", L"Button", MB_OK);
				switch (HIWORD(wParam)) {
				case BN_CLICKED:
					MessageBox(hWnd, L"��ư1�� Ŭ�� �Ͽ����ϴ�.", L"button", MB_OK);
					break;
				case BN_SETFOCUS:
					MessageBox(hWnd, L"��ư1�� ��Ŀ�� �Ͽ����ϴ�..", L"button", MB_OK);
					break;
				}
				break;
			case 200:
				MessageBox(hWnd, L"��ư2�� �̺�Ʈ�� �߻����׽��ϴ�.", L"Button", MB_OK);
				break;

			case ID_CHK_RETANGLE:
				if (SendMessage(hChkRectangle, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {	//���
					SendMessage(hChkRectangle, BM_SETCHECK, BST_CHECKED, 0);
					bRectangle = TRUE;
				}
				else {
					SendMessage(hChkRectangle, BM_SETCHECK, BST_UNCHECKED, 0);
					bRectangle = FALSE;
				}
				InvalidateRect(hWnd, NULL, TRUE);			//�׷����� ������ ��ȭ�� ȭ�鿡 �����ϱ� ����.
				break;
			case ID_CHK_ELLIPSE:
				if (SendMessage(hChkEllipse, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {			//���
					SendMessage(hChkEllipse, BM_SETCHECK, BST_CHECKED, 0);
					bEllipse = TRUE;
				}
				else {
					SendMessage(hChkEllipse, BM_SETCHECK, BST_UNCHECKED, 0);
					bEllipse = FALSE;
				}
				InvalidateRect(hWnd, NULL, TRUE);
				break;

			case ID_RADIO1:
				nGraph = 0;
				break;
			case ID_RADIO2:
				nGraph = 1;
				break;
			case ID_RADIO3:
				nColor = 0;
				break;
			case ID_RADIO4:
				nColor = 1;
				break;
			case ID_RADIO5:
				nColor = 2;
				break;

			case ID_EDIT:
				switch (HIWORD(wParam)) {
				case EN_CHANGE:
					GetWindowText(g_hEdit, (LPWSTR)strEdit, 128);
					SetWindowText(g_hStatic2, (LPWSTR)strEdit);
				}
				break;
			case ID_LIST:
				switch (HIWORD(wParam)) {
				case LBN_SELCHANGE:
					nGetSel = SendMessage(g_hList, LB_GETCURSEL, 0, 0);
					SendMessage(g_hList, LB_GETTEXT, nGetSel, (LPARAM)strList);
					SetWindowText(g_hStatic2, strList);
				}
				break;
				
			case ID_COMBO:
				switch (HIWORD(wParam)) {
				case CBN_SELCHANGE:
					nGetSel = SendMessage(g_hCombo, CB_GETCURSEL, 0, 0);
					SendMessage(g_hCombo, CB_GETLBTEXT, nGetSel, (LPARAM)strList);
					SetWindowText(g_hStatic2, strList);
				}
				break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (bRectangle == TRUE)		Rectangle(hdc, 150, 120, 250, 200);
		if (bEllipse== TRUE)		Ellipse(hdc, 150, 120, 250, 200);
		switch (nColor)		//��
		{
		case 0:
			MyBrush = CreateSolidBrush(RGB(255, 0, 0));
			break;
		case 1:
			MyBrush = CreateSolidBrush(RGB(0,255, 0));
			break;
		case 2:
			MyBrush = CreateSolidBrush(RGB(0,0,255));
			break;
		}
		OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);

		if (nScrollMove != 0) {
			MyBrush = CreateSolidBrush(RGB(nScrollMove, nScrollMove, nScrollMove));
			OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
		}

		switch (nGraph)
		{
		case 0:
			Rectangle(hdc, 10, 400, 200, 500);
			break;
		case 1:
			Ellipse(hdc, 10, 400, 200, 500);
			break;
		}

		SelectObject(hdc, OldBrush);
		DeleteObject(MyBrush);

		EndPaint(hWnd, &ps);
		return 0;

	case WM_HSCROLL:
		if ((HWND)lParam == g_hScroll) {
			switch (LOWORD(wParam)) {
				nScrollMove = HIWORD(wParam);
			case SB_LINELEFT:
				nScrollMove = max(0, nScrollMove - 1);
				break;
			case SB_LINERIGHT:
				nScrollMove = min(255, nScrollMove + 1);
				break;
			case SB_PAGELEFT:
				nScrollMove = max(0, nScrollMove - 10);
				break;
			case SB_PAGERIGHT:
				nScrollMove = min(255, nScrollMove + 10);
				break;
			case SB_THUMBTRACK:		//�巡��
				nScrollMove = HIWORD(wParam);
				break;
			}
		}
		SetScrollPos(g_hScroll, SB_CTL, nScrollMove, TRUE);
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}