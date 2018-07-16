#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"GdiObject";

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

	HDC hdc, hMetaDC;
	PAINTSTRUCT ps;
	HENHMETAFILE hMFile;
	RECT rt;
	LOGBRUSH lb;

	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		hMetaDC = CreateEnhMetaFile(hdc, L"GdiObjectMetaFile.emf", NULL, NULL);
		SelectObject(hMetaDC, CreateSolidBrush(RGB(0, 0, 255)));
		lb.lbStyle = BS_SOLID;
		lb.lbColor = RGB(255, 0, 0);
		lb.lbHatch = 0;

		SelectObject(hMetaDC, ExtCreatePen(PS_SOLID | PS_GEOMETRIC, 5, &lb, 0, NULL));

		Rectangle(hMetaDC, 0, 0, 300, 300);
		Ellipse(hMetaDC, 0, 0, 300, 300);
		MoveToEx(hMetaDC, 0, 0, NULL);
		LineTo(hMetaDC, 300, 300);
		MoveToEx(hMetaDC, 0, 300, NULL);
		LineTo(hMetaDC, 300, 0);

		DeleteObject(SelectObject(hMetaDC, GetStockObject(BLACK_PEN)));
		DeleteObject(SelectObject(hMetaDC, GetStockObject(WHITE_BRUSH)));

		hMFile = CloseEnhMetaFile(hMetaDC);
		DeleteEnhMetaFile(hMFile);
		ReleaseDC(hWnd, hdc);
		return 0;

	case WM_RBUTTONDOWN:
		hdc = GetDC(hWnd);
		hMFile = GetEnhMetaFile(L"GdiObjectMetaFile.emf");
		if (hMFile == NULL)
		{
			MessageBox(hWnd, L"파일을 찾을 수 없습니다.", L"알림", MB_OK);
		}
		else
		{
			rt.left = LOWORD(lParam);
			rt.right = rt.left + 100;
			rt.top = HIWORD(lParam);
			rt.bottom = rt.top + 100;
			PlayEnhMetaFile(hdc, hMFile, &rt);
		}
		PlayEnhMetaFile(hdc, hMFile, &rt);
		DeleteEnhMetaFile(hMFile);
		ReleaseDC(hWnd, hdc);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}