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


//����Ʈ�ڽ���ID
#define ID_EDIT	  501

//����Ʈ �ڽ��� ID
#define ID_LIST   601

//����Ʈ�ڽ����ڵ�
HWND g_hEdit;

//����Ʈ�ڽ����ڵ�
HWND g_hList;

//��Ʈ ����
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
		clientsock = accept(*P, (sockaddr*)&addr_client, &addrlen_clt);	//Ŭ���̾�Ʈ ���� ���

		if (clientsock == INVALID_SOCKET)
		{
			continue;
		}
		else
		{
			SendMessage(g_hList, LB_ADDSTRING, 0, (LPARAM)"Ŭ���̾�Ʈ�� �����Ͽ����ϴ�.");
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
				sprintf(strTemp, "���� ���� : %s", buf);
				SendMessage(g_hList, LB_ADDSTRING, 0, (LPARAM)strTemp);
			}
			else
			{
				sprintf(strTemp, "������ �޽��� : %s", buf);
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

	//����Ʈ �ڽ� ����
	char strEdit[128];

	//����Ʈ �ڽ� �� �޺� �ڽ� ���� ����
	char strList[128];

	DWORD ThreadID;
	static HANDLE hThread;

	char strMes[128] = "���� �����Դϴ�.";

	switch (iMessage)
	{
	case WM_CREATE:
		nReturn = WSAStartup(WORD(2.0), &wsadata);	//���� ���̺귯�� �ʱ�ȭ

		listensock = socket(AF_INET, SOCK_STREAM, 0);

		addr_server.sin_family = AF_INET;
		addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
		addr_server.sin_port = htons(g_uPort);
		nReturn = bind(listensock, (sockaddr*)&addr_server, sizeof(sockaddr));	//���Ͽ� �ּ�, ��������, ��Ʈ �Ҵ�

		nReturn = listen(listensock, 1);	//�����û ������ ����ϴ� ���� ��⿭ ����

		hThread = CreateThread(NULL, 0, ThreadFunc, &listensock, 0, &ThreadID);
		///////// ������ ��ư ���� /////////
		CreateWindow("button", "������", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			230, 140, 80, 25, hWnd, (HMENU)200, g_hInst, NULL);

		/////////// ����Ʈ ������ ���� //////////
		g_hEdit = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE |
			WS_BORDER | ES_AUTOVSCROLL | ES_MULTILINE, 20, 110, 300, 20, hWnd, (HMENU)ID_EDIT, g_hInst, NULL);

		/////////// ����Ʈ �ڽ� ���� //////////
		g_hList = CreateWindow("listbox", NULL, WS_CHILD | WS_VISIBLE |
			WS_BORDER | WS_VSCROLL, 20, 40, 300, 80, hWnd, (HMENU)ID_LIST, g_hInst, NULL);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			//////////////////// �Ϲ� ��ư �̺�Ʈ S///////////////////////
		case 100:
			nReturn = connect(clientsock, (sockaddr*)&addr_client, addrlen_clt);
			if (!nReturn)
				SendMessage(g_hList, LB_ADDSTRING, 0, (LPARAM)"������ �����Ͽ����ϴ�.");
			break;
		case 200:
			GetWindowText(g_hEdit, strEdit, 128);
			nReturn = send(clientsock, strEdit, sizeof(strEdit), 0);	//������ �ۼ���
			if (nReturn)
			{
				sprintf(buf, "�۽��� �޽��� : %s", strEdit);
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
		closesocket(clientsock);	//���� ����
		WSACleanup();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
