#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const char* lpszClass = "SocketClient";

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
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

//에디트박스의ID
#define ID_EDIT	  501

//리스트 박스의 ID
#define ID_LIST   601

//에디트박스의핸들
HWND g_hEdit;

//리스트박스의핸들
HWND g_hList;

const char g_szlpAddress[17] = "127.0.0.1";
const unsigned short g_uPort = 7878;

int nReturn;
WSADATA wsadata;
SOCKET		clientsock;
sockaddr_in	addr_client;
int					addrlen_clt = sizeof(sockaddr);
char				buf[1024];
char				strTemp[1024];

DWORD WINAPI ThreadFunc(LPVOID Param)
{
	HDC hdc;
	BYTE Red = 0;
	BYTE Green = 0;
	BYTE Blue = 0;
	HBRUSH hBrush, hOldBrush;
	SOCKET* P = (SOCKET*)Param;
	for (;;)
	{
		unsigned long argp = 1;
		{
			nReturn = recv(*P, buf, 1024, 0);	//데이터 수신(buffer에 저장된다.)
			if (nReturn == 0 || nReturn == SOCKET_ERROR)
			{
				continue;
			}
			else
			{
				sprintf(strTemp, "수신한 메시지 : %s", buf);
				SendMessage(g_hList, LB_ADDSTRING, 0, (LPARAM)strTemp);
			}
		}
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	//에디트 박스 변수
	char strEdit[128];

	//리스트 박스 변수
	char strList[128];

	DWORD ThreadID;
	static HANDLE hThread;

	char strMes[128] = "소켓 클라이언트입니다.";

	switch (iMessage)
	{
	case WM_CREATE:
		nReturn = WSAStartup(WORD(2.0), &wsadata);

		clientsock = socket(AF_INET, SOCK_STREAM, 0);

		addr_client.sin_family = AF_INET;
		addr_client.sin_addr.s_addr = inet_addr(g_szlpAddress);
		addr_client.sin_port = htons(g_uPort);

		hThread = CreateThread(NULL, 0, ThreadFunc, &clientsock, 0, &ThreadID);
		///////// 일반 버튼 생성 /////////
		CreateWindow("button", "접속하기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			40, 140, 80, 25, hWnd, (HMENU)100, g_hInst, NULL);

		CreateWindow("button", "보내기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			230, 140, 80, 25, hWnd, (HMENU)200, g_hInst, NULL);

		/////////// 에디트 윈도우 생성 //////////
		g_hEdit = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE |
			WS_BORDER | ES_AUTOVSCROLL | ES_MULTILINE, 20, 110, 300, 20, hWnd, (HMENU)ID_EDIT, g_hInst, NULL);

		/////////// 리스트 박스 생성 //////////
		g_hList = CreateWindow("listbox", NULL, WS_CHILD | WS_VISIBLE |
			WS_BORDER | WS_VSCROLL, 20, 40, 300, 80, hWnd, (HMENU)ID_LIST, g_hInst, NULL);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			//////////////////// 일반 버튼 이벤트 S///////////////////////
		case 100:
			nReturn = connect(clientsock, (sockaddr*)&addr_client, addrlen_clt);	//서버에 접속
			if (!nReturn)
				SendMessage(g_hList, LB_ADDSTRING, 0, (LPARAM)"서버에 접속하였습니다.");
			break;
		case 200:
			GetWindowText(g_hEdit, strEdit, 128);
			nReturn = send(clientsock, strEdit, sizeof(strEdit), 0);	//데이터 송수신
			if (nReturn)
			{
				sprintf(buf, "송신한 메시지 : %s", strEdit);
				SendMessage(g_hList, LB_ADDSTRING, 0, (LPARAM)buf);
			}
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 20, 20, strMes, strlen(strMes));
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		closesocket(clientsock);
		WSACleanup();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
