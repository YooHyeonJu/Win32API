#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"OutputDIB";

PBYTE PbPixmap;
BITMAPFILEHEADER bfh;
BITMAPINFOHEADER bih;


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

	HANDLE hFile;
	DWORD dwRead;
	DWORD dwSize;
	TCHAR strFileName[MAX_PATH] = L"BITMAP4.bmp";

	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		hFile = CreateFile(strFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, L"파일 생성 실패", L"알림", MB_OK);
			return 0;
		}

		ReadFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwRead, NULL);
		ReadFile(hFile, &bih, sizeof(BITMAPINFOHEADER), &dwRead, NULL);
		dwSize = GetFileSize(hFile, NULL);
		PbPixmap = new BYTE[dwSize];
		ReadFile(hFile, PbPixmap, dwSize, &dwRead, NULL);
		CloseHandle(hFile);
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//SetDIBitsToDevice(hdc, 0, 0, bih.biWidth, bih.biHeight, 0, 0, 0, bih.biHeight, PbPixmap, (LPBITMAPINFO)&bih, DIB_RGB_COLORS);
		SetStretchBltMode(hdc, COLORONCOLOR);
		StretchDIBits(hdc, 0, 0, bih.biWidth/2, bih.biHeight/2, 0, 0,bih.biWidth, bih.biHeight, PbPixmap, (LPBITMAPINFO)&bih, DIB_RGB_COLORS,SRCCOPY);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}