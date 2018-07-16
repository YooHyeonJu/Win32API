#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const char* lpszClass = "SocketServer";

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

//포트 설정
const unsigned short g_uPort = 7878;

int nReturn;
WSADATA wsadata;
SOCKET		listensock;
SOCKET		clientsock = 0;
sockaddr_in	addr_server;
sockaddr_in	addr_client;
int					addrlen_srv = sizeof(sockaddr);
int					addrlen_clt = sizeof(sockaddr);
in_addr			in;
hostent*			ht;
const int		buflen = 4096;
char				buf[buflen];
char				strTemp[buflen];

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
		//		if(!clientsock)
		clientsock = accept(*P, (sockaddr*)&addr_client, &addrlen_clt);	//클라이언트 접속 허용

		if (clientsock == INVALID_SOCKET)
		{
			continue;
		}
		else
		{
			SendMessage(g_hList, LB_ADDSTRING, 0, (LPARAM)"클라이언트가 접속하였습니다.");
		}

		//			unsigned long argp = 1;	
		//			nReturn = ioctlsocket(clientsock, FIONBIO, &argp);
		//			while(1)
		{
			//				Sleep(1000);
			nReturn = recv(clientsock, buf, buflen, 0);
			if (nReturn == 0 || nReturn == SOCKET_ERROR)
			{
				//					if(WSAGetLastError() == WSAEWOULDBLOCK)
				//						continue;
				sprintf(strTemp, "수신 에러 : %s", buf);
				SendMessage(g_hList, LB_ADDSTRING, 0, (LPARAM)strTemp);
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

	//리스트 박스 및 콤보 박스 공통 변수
	char strList[128];

	DWORD ThreadID;
	static HANDLE hThread;

	char strMes[128] = "소켓 서버입니다.";

	switch (iMessage)
	{
	case WM_CREATE:
		nReturn = WSAStartup(WORD(2.0), &wsadata);	//소켓 라이브러리 초기화

		listensock = socket(AF_INET, SOCK_STREAM, 0);

		addr_server.sin_family = AF_INET;
		addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
		addr_server.sin_port = htons(g_uPort);
		nReturn = bind(listensock, (sockaddr*)&addr_server, sizeof(sockaddr));	//소켓에 주소, 프로토콜, 포트 할당

		nReturn = listen(listensock, 1);	//연결요청 소켓이 대기하는 연결 대기열 생성

		hThread = CreateThread(NULL, 0, ThreadFunc, &listensock, 0, &ThreadID);
		///////// 보내기 버튼 생성 /////////
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
			nReturn = connect(clientsock, (sockaddr*)&addr_client, addrlen_clt);
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
		closesocket(clientsock);	//접속 종료
		WSACleanup();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
