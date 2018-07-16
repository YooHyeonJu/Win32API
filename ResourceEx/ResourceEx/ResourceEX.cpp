#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include<time.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"ResourceEx";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	HACCEL hMyAccel;
	hMyAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MYACCEL));	//������������ ���̺��� �о���δ�.
	

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_MYICON));
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU2);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0)) {
		if (!TranslateAccelerator(hWnd, hMyAccel, &Message)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	HDC hdc;
	PAINTSTRUCT ps;
	time_t mytime;			//�ð� ���ڿ��� �����ϴ� ����
	TCHAR strTemp[256];
	//static const wchar_t* strtime = (const wchar_t*)L"";
	static TCHAR* strtime;

	HMENU MyPopup1;		//�˾� �޴��� ����� ���� �ڵ�
	HMENU MyPopup2;		//�˾� �޴��� ����� ���� �ڵ�
	POINT point;		//���콺�� Ŭ���ϴ� ����
	switch (iMessage)
	{
	case WM_CREATE:
		strtime = (TCHAR*)L"";
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_CUR_TIME:
			time(&mytime);
			strtime = _wctime(&mytime);
			//time(&mytime);
			//strtime = asctime(ctime(&mytime));
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_POPUP_FIRST:
			MessageBox(hWnd, L"ù ��° �˾��޴��� �����Ͽ����ϴ�.", L"�˾� �޴�",MB_OK);
			break;
		case ID_POPUP_SECOND:
			MessageBox(hWnd, L"�� ��° �˾��޴��� �����Ͽ����ϴ�.", L"�˾� �޴�", MB_OK);
			break;
		case ID_ACCELERATOR:
			MessageBox(hWnd, L"�����������͸� �����Ͽ����ϴ�.", L"������������", MB_OK);
			break;
		}
		return 0;
		/*
		case WM_RBUTTON:
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			MyPopup1 = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_POPUP));
			MyPopup2 = GetSubMenu(MyPopup1, 0);
			ClientToScreen(hWnd,&point);
			TrackPopupMenu(MyPopup2, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
			return 0;
		*/
	case WM_CONTEXTMENU:
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		MyPopup1 = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_POPUP));	//�˾��޴� �ڵ�ޱ�.
		MyPopup2 = GetSubMenu(MyPopup1, 0);			//���� �޴� �� ���� �޴� ����
		TrackPopupMenu(MyPopup2, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
				//�޾ƿ� ��ġ�� �˾�����
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//TextOut(hdc, 100, 100, (LPCWSTR)strtime, wcslen(strtime)-1);
		//TextOut(hdc, 100, 100, (LPCWSTR)temp, strlen(temp) - 1);
		TextOut(hdc, 100, 100, strtime, wcslen(strtime) - 1);
		LoadString(g_hInst, IDS_MYSTRING, strTemp, 256);
		TextOut(hdc, 100, 200, strTemp, wcslen(strTemp) - 1);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}