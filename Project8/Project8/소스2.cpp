#include <windows.h>
#include <tchar.h>
#include<commctrl.h>
//#include<CommCtrl.h>
#include"resource.h"

LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK CloseEnumProc(HWND, LPARAM);	//모든 자식 윈도우를 일괄적으로 관리

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MAINMENU);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	InitCommonControls();

	//자식 윈도우 클래스 등록
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)ChildWndProc;	//클래스 이름
	WndClass.lpszClassName = lpszChildClass;		//프로시저
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	//클래스 이름으로 윈도우 커널에서 다른 것을 구분한다.


	//InitCommonControls();//공용 컨트롤 초기화 루틴

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	hChildWnd = GetWindow(hWnd, GW_CHILD);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	while (GetMessage(&Message, 0, 0, 0)) {
		if (!TranslateMDISysAccel(hChildWnd, &Message))	//MDI기반에서 자식 윈도우의 메시지를 받기 위한 함수.
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
	static HWND toolbar;

	static HWND hClient;
	CLIENTCREATESTRUCT clientcreate;

	HWND hChild;
	MDICREATESTRUCT mdicreate;

	switch (iMessage)
	{
	case WM_CREATE:	//프레임 윈도우 생성시 클라이언트 윈도우 같이 생성함.
		ptb = tbb;
		toolbar = CreateToolbarEx(hWnd, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_BORDER | CCS_TOP, 1, 1, g_hInst, IDB_BITMAP, ptb, 7, 16, 16, 16, 15, sizeof(TBBUTTON));
		clientcreate.hWindowMenu = NULL;
		clientcreate.idFirstChild = 100;
		hClient = CreateWindow(L"MDICLIENT", NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)1, g_hInst, (LPWSTR)&clientcreate);
		//NULL : 메뉴 핸들을 프레임 윈도우에 직접 등록하므로 사용X
		//WS_CHILDREN : 클라이언트 윈도우는 프레임 윈도우에 붙어서 동작.
		return 0;
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_FILENEW:
			mdicreate.szClass = lpszChildClass;
			mdicreate.szTitle = L"ChildView";
			mdicreate.hOwner = g_hInst;
			mdicreate.x = CW_USEDEFAULT;
			mdicreate.y = CW_USEDEFAULT;
			mdicreate.cx = CW_USEDEFAULT;
			mdicreate.cy = CW_USEDEFAULT;
			mdicreate.style = 0;
			mdicreate.lParam = 0;

			hChild = (HWND)SendMessage(hClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate);
			break;
		case ID_CLOSE:
			hChild = (HWND)SendMessage(hClient, WM_MDIGETACTIVE, 0, 0);
			if (SendMessage(hChild, WM_QUERYENDSESSION, 0, 0))
				SendMessage(hClient, WM_MDIDESTROY, (WPARAM)hChild, 0);
			break;
		case ID_CLOSEALL:
			EnumChildWindows(hClient, &CloseEnumProc, 0);
			break;
		case ID_EXIT:
			SendMessage(hWnd, WM_DESTROY, 0, 0);
			break;

		case ID_TILE:
			SendMessage(hClient, WM_MDITILE, 0, 0);
			break;
		case ID_CASCADE:
			SendMessage(hClient, WM_MDICASCADE, 0, 0);
			break;
		case ID_ICONARRANGE:
			SendMessage(hClient, WM_MDIICONARRANGE, 0, 0);
			break;

		}
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