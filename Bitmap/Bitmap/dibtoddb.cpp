#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"DIB_to_DDB";

BITMAPFILEHEADER* pfh;
PVOID pih;

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

	HDC hdc,hMemDC;
	PAINTSTRUCT ps;
	static HBITMAP hBitmap;
	HBITMAP OldBitmap;
	TCHAR strPath[MAX_PATH];

	HANDLE hFile;
	DWORD FileSize, dwRead;
	BITMAP bitmap;

	switch (iMessage)
	{

	case WM_LBUTTONDOWN:
		GetCurrentDirectory(MAX_PATH, strPath);
		wcscat(strPath, L"\\sky.bmp");
		hdc = GetDC(hWnd);

		hFile = CreateFile(strPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, L"파일 생성 실패", L"알림", MB_OK);
			return 0;
		}

		FileSize = GetFileSize(hFile, NULL);
		pfh = (BITMAPFILEHEADER*)malloc(FileSize);
		ReadFile(hFile, pfh, FileSize, &dwRead, NULL);
		CloseHandle(hFile);
		pih = ((PBYTE)pfh + sizeof(BITMAPFILEHEADER));
		
		//DC에 GDI객체를 선택하여 DIB를 그림(DDB) =>DIB가 느리기 때문에 DDB로 변형
		hBitmap = CreateDIBitmap(hdc, (BITMAPINFOHEADER*)pih, CBM_INIT, (PBYTE)pfh + pfh->bfOffBits, (BITMAPINFO*)pih, DIB_RGB_COLORS);
		ReleaseDC(hWnd, hdc);
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hMemDC = CreateCompatibleDC(hdc);	///GDI 객체 생성
		OldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);
		BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);
		SelectObject(hMemDC, OldBitmap);
		DeleteDC(hMemDC);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		free(pfh);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}