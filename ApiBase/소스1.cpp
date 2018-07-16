#include <windows.h>
#include <tchar.h>

//#define WS_OVERLAPPEDWINDOW WS_CAPTION | WS_SYSMENU
//WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_BORDER | WS_CHILD | WS_POPUP 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"ApiBase";


/////1.클래스 생성

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;		//클래스 여분 바이트 수
	WndClass.cbWndExtra = 0;		//윈도우 여분 바이트 수
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//윈도우 배경색
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		//프로그램에서 사용할 커서
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//사용할 아이콘
	WndClass.hInstance = hInstance;				//인스턴스 핸들
	WndClass.lpfnWndProc = (WNDPROC)WndProc;		//프로시저 함수명
	WndClass.lpszClassName = lpszClass;			//구조체로 만들어질 클래스명
	WndClass.lpszMenuName = NULL;				//프로그램에서 사용할 메뉴
	WndClass.style = CS_HREDRAW | CS_VREDRAW;		//윈도우 스타일

	RegisterClass(&WndClass);	//클래스의 등록


								////2.클래스 객체 생성

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);

	
	////3.윈도우 객체 화면에 띄우기

	ShowWindow(hWnd, nCmdShow);

	DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL;
	SetWindowLong(hWnd, GWL_STYLE, dwStyle);
	SendMessage(hWnd, WM_NCPAINT, 1, 0);


	////4.메시지 루프 돌리기 -> 윈도우 프로시저 함수로 메시지를 전달한다.

	while (GetMessage(&Message, 0, 0, 0)) {	//거짓일 때까지 반복문을 돈다. (WM-QUIT이면 GetMessage 함수는 거짓 반환)
		TranslateMessage(&Message);	//문자 입력을 처리하는 함수
		DispatchMessage(&Message);	//GetMessage 함수로부터 전달된 메시지를 윈도우 프로시저로 보내는 함수
									//	(메시지의 완전한 처리 전까지는 반환되지 않는다.)
	}
	return Message.wParam;
}

////5.메시지 처리

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	HDC hdc;
	PAINTSTRUCT ps;
	LONG dwStyle;

	/////////////윈도우 스타일 변경
	TCHAR strTitle[] = L"윈도우 스타일 변경";


	switch (iMessage)
	{
	case WM_CREATE:
		return 0;
	case WM_LBUTTONDOWN:
		dwStyle = GetWindowLong(hWnd, GWL_STYLE);
		dwStyle = dwStyle | WS_VSCROLL | WS_HSCROLL;
		SetWindowLong(hWnd, GWL_STYLE, dwStyle);
		SendMessage(hWnd, WM_NCPAINT, 1, 0);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 10, 10, strTitle, wcslen(strTitle));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}