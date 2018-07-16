#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"MCI";

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
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

UINT wDeviceID = 0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	MCI_OPEN_PARMS	mciOpenParams;
	MCI_PLAY_PARMS	mciPlayParams;
	MCI_RECORD_PARMS mciRecordParams;
	MCI_SAVE_PARMS	mciSaveParams;
	DWORD dwResult;
	TCHAR strMes[128];

	switch (iMessage)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_MCIOPEN:
			mciOpenParams.lpstrDeviceType = L"WaveAudio";
			//mciOpenParams.lpstrElementName = L"together.wav";
			mciOpenParams.lpstrElementName = L"wawawa.wav";
			dwResult = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpenParams);
			if (dwResult)
			{
				mciSendCommand(wDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
				mciGetErrorString(dwResult, strMes, 128);
				MessageBox(hWnd, strMes, L"오류 메시지", MB_OK);
			}
			wDeviceID = mciOpenParams.wDeviceID;
			break;
		case ID_MCIPLAY:
			mciPlayParams.dwCallback = (DWORD)hWnd;
			dwResult = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlayParams);
			if (dwResult)
			{
				mciSendCommand(wDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
				mciGetErrorString(dwResult, strMes, 128);
				MessageBox(hWnd, strMes, L"오류 메시지", MB_OK);
			}
			break;
		case ID_MCIPAUSE:
			mciPlayParams.dwCallback = (DWORD)hWnd;
			mciSendCommand(wDeviceID, MCI_PAUSE, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlayParams);
			break;
		case ID_MCIRECORD:
			mciRecordParams.dwTo = 0;
			mciSendCommand(wDeviceID, MCI_RECORD, MCI_TO | MCI_WAIT, (DWORD)(LPVOID)&mciRecordParams);
			break;
		case ID_MCIRECORDSAVE:
			mciSaveParams.lpfilename = L"c:\\wa.wav";
			mciSendCommand(wDeviceID, MCI_SAVE, MCI_SAVE_FILE | MCI_WAIT, (DWORD)(LPVOID)&mciSaveParams);
			break;
		case ID_MCISTART:
			mciSendCommand(wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
			break;
		case ID_MCIEND:
			mciSendCommand(wDeviceID, MCI_SEEK, MCI_SEEK_TO_END, (DWORD)(LPVOID)NULL);
			break;
		}
		return 0;
	case MM_MCINOTIFY:
		switch (wParam)
		{
		case MCI_NOTIFY_SUCCESSFUL:
			mciSendCommand(LOWORD(lParam), MCI_CLOSE, 0, (DWORD)NULL);
			break;
		case MCI_NOTIFY_FAILURE:
			mciSendCommand(LOWORD(lParam), MCI_CLOSE, 0, (DWORD)NULL);
			break;
		case MCI_NOTIFY_SUPERSEDED:
			mciSendCommand(LOWORD(lParam), MCI_CLOSE, 0, (DWORD)NULL);
			break;
		case MCI_NOTIFY_ABORTED:
			mciSendCommand(LOWORD(lParam), MCI_CLOSE, 0, (DWORD)NULL);
			break;
		}
		return 0;
	case WM_DESTROY:
		if (wDeviceID)
			mciSendCommand(LOWORD(lParam), MCI_CLOSE, 0, (DWORD)NULL);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}