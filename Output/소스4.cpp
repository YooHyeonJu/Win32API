#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"ApiBase";

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
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
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
	POINT polyline[5] = { 50,133,146,99,246,133,247,212,58,216 };
	//점이 만나려면,
	//POINT polyline[6] = { 50,133,146,99,246,133,247,212,58,216,50,133 };

	POINT polygon[5] = { 350,133,446,99,546,133,547,212,358,216 };

	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		/*
		//점
		hdc = BeginPaint(hWnd, &ps);
		SetPixel(hdc, 100, 100, RGB(255, 0, 0));
		EndPaint(hWnd, &ps);
		return 0;


		//선
		hdc = BeginPaint(hWnd, &ps);
		MoveToEx(hdc, 100, 100, NULL);		//시작점  NULL자리는 이전 설정된 그리기 좌표 값을 설정해 주는 구조체 포인터
		LineTo(hdc, 400, 200);				//끝점
		EndPaint(hWnd, &ps);
		return 0;


		//사각형
		hdc = BeginPaint(hWnd, &ps);
		Rectangle(hdc, 100, 100, 300, 300);
		RoundRect(hdc, 400, 100, 600, 300, 20, 20);		//20은 가로,세로축의 지름이다.
		EndPaint(hWnd, &ps);
		return 0;


		//원
		hdc = BeginPaint(hWnd, &ps);
		Ellipse(hdc, 100, 100, 200, 200);
		Arc(hdc, 300, 100, 400, 200, 350, 100, 400, 150);
		Chord(hdc, 100, 300, 200, 400, 150, 300, 100, 350);
		Pie(hdc, 300, 300, 400, 400, 350, 300, 300, 350);
		EndPaint(hWnd, &ps);
		return 0;
		*/

		hdc = BeginPaint(hWnd, &ps);
		Polyline(hdc, polyline, 5);
		//점이 만나려면,
		//Polyline(hdc,polyline,6);

		Polygon(hdc, polygon, 5);
		EndPaint(hWnd, &ps);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}