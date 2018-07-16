#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPWSTR lpszClass = L"ResourceEx";

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
	//WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_MYCURSOR));
	//WndClass.hIcon=LoadIcon(NULL,IDI_WARNING);
	WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	//WndClass.lpszMenuName=NULL;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	HACCEL hMyAccel;
	hMyAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MYACCEL));

	while (GetMessage(&Message, 0, 0, 0))
	{
		if (!TranslateAccelerator(hWnd, hMyAccel, &Message))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	return Message.wParam;
}
#include <time.h>
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	time_t mytime;
	static TCHAR* strtime;
	PAINTSTRUCT ps;

	HMENU MyPopup1;
	HMENU MyPopup2;
	POINT point;
	TCHAR strTemp[256];

	switch (iMessage)
	{
	case WM_CREATE:
		strtime = L"";
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_CUR_TIME:
			time(&mytime);
			strtime = _wctime(&mytime);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_POPUP_FIRST:
			MessageBox(hWnd, L"첫번째 팝업 메뉴를 선택하였습니다.", L"팝업 메뉴", MB_OK);
			break;
		case ID_POPUP_SECOND:
			MessageBox(hWnd, L"두번째 팝업 메뉴를 선택하였습니다.", L"팝업 메뉴", MB_OK);
			break;
		case ID_ACCELERATOR:
			MessageBox(hWnd, L"엑셀러레이터를 선택하였습니다.", L"엑셀러레이터", MB_OK);
			break;
		}
		return 0;
	case WM_CONTEXTMENU:
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		MyPopup1 = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_POPUP));
		MyPopup2 = GetSubMenu(MyPopup1, 0);
		TrackPopupMenu(MyPopup2, TPM_LEFTALIGN | TPM_RIGHTBUTTON, LOWORD(lParam), HIWORD(lParam), 0, hWnd, NULL);
		return 0;
		/*case WM_RBUTTONDOWN:
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		MyPopup1 = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_POPUP));
		MyPopup2 = GetSubMenu(MyPopup1, 0);
		ClientToScreen(hWnd,&point);
		TrackPopupMenu(MyPopup2, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
		return 0;*/
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 100, strtime, wcslen(strtime) - 1);
		LoadString(g_hInst, IDS_MYSTRING, strTemp, 256);
		TextOut(hdc, 100, 300, strTemp, wcslen(strTemp) - 1);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
