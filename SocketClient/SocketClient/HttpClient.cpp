#include <windows.h>
#include <wininet.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const char* lpszClass = "HttpClient";

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

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

#define ID_EDIT1 100
#define ID_EDIT2 200
#define ID_BUTTON 300

HWND hEdit1;
HWND hEdit2;
HWND hButton;

int GetHttpFile(char *url)
{
	HINTERNET hInternet, hHttp;
	HINTERNET hReq;
	HINTERNET hFile;
	DWORD	size;
	DWORD ReadSize;
	BOOL bResult;
	int buff_cnt = 0;
	char buf[65000];
	char buf2[650000] = { 0, };
	DWORD dwWritten;
	char szBuf[256];

	hInternet = InternetOpen("HTTP Client", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	hHttp = InternetConnect(hInternet, url, 0, "", "", INTERNET_SERVICE_HTTP, 0, 0);
	if (hHttp == NULL)
		return 0;

	hReq = HttpOpenRequest(hHttp, "GET", "", NULL, NULL, NULL, 0, 0);
	HttpSendRequest(hReq, NULL, 0, NULL, 0);

	for (;;)
	{
		InternetQueryDataAvailable(hReq, &size, 0, 0);

		bResult = InternetReadFile(hReq, buf, size, &ReadSize);

		buf[ReadSize] = 0;
		strcat(buf2, buf);

		if (ReadSize == 0)break;
	}

	SetWindowText(hEdit1, buf2);

	memset(szBuf, 0, 256);
	GetCurrentDirectory(256, szBuf);
	strcat(szBuf, "\\index.html");

	hFile = CreateFile(szBuf, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, buf2, strlen(buf2), &dwWritten, NULL);
	CloseHandle(hFile);

	InternetCloseHandle(hHttp);
	InternetCloseHandle(hInternet);
	return buff_cnt;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	char strMes[] = "웹 주소를 입력하고, 다운로드 버튼을 누르세요.";
	char szURL[256];

	switch (iMessage)
	{
	case WM_CREATE:
		hEdit1 = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |
			ES_MULTILINE, 10, 10, 600, 500, hWnd, (HMENU)ID_EDIT1, g_hInst, NULL);

		hEdit2 = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE |
			WS_BORDER | ES_AUTOVSCROLL | ES_MULTILINE, 620, 50, 200, 20, hWnd, (HMENU)ID_EDIT2, g_hInst, NULL);

		hButton = CreateWindow("button", "다운로드", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			830, 50, 80, 25, hWnd, (HMENU)ID_BUTTON, g_hInst, NULL);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_BUTTON:
			GetWindowText(hEdit2, szURL, 256);
			GetHttpFile(szURL);
			break;
		}
		return 0;
	case WM_SIZE:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 620, 20, strMes, strlen(strMes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
