#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"InstallFont";

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
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

HWND hWndMain;

BOOL SHRegWriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData)
{
	HKEY key;
	DWORD dwDisp;
	if (RegCreateKeyEx(hKey, lpKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, &dwDisp) != ERROR_SUCCESS)
		return FALSE;
	if (RegSetValueEx(key, lpValue, 0, REG_SZ, (LPBYTE)lpData, wcslen(lpData) + 1) != ERROR_SUCCESS)
		return FALSE;
	RegCloseKey(key);
	return TRUE;
}

void InstallFont()
{
	TCHAR FontPath[MAX_PATH];
	TCHAR FilePath[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, FilePath);
	if (wcslen(FilePath) == 3)
	{
		wcscat(FilePath, L"JI Pumpkins.ttf");
	}
	else
	{
		wcscat(FilePath, L"\\JI Pumpkins.ttf");
	}

	GetWindowsDirectory(FontPath, MAX_PATH);
	wcscat(FontPath, L"\\Fonts\\JI Pumpkins.ttf");

	if (_waccess(FontPath, 0) != 0)
	{
		if (CopyFile(FilePath, FontPath, FALSE) == FALSE)
		{
			MessageBox(hWndMain, L"폰트 복사에 실패하였습니다.", L"알림", MB_OK);
			InvalidateRect(hWndMain, NULL, TRUE);
			return;
		}
	}

	if (AddFontResource(FontPath) == FALSE)
	{
		MessageBox(hWndMain, L"폰트 등록에 실패하였습니다.", L"알림", MB_OK);
		return;
	}
	SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

	OSVERSIONINFO vi;
	vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	TCHAR szKey[255];
	GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		wcscpy(szKey, L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts");
	}
	else
	{
		wcscpy(szKey, L"Software\\Microsoft\\Windows\\CurrentVersion\\Fonts");
	}

	SHRegWriteString(HKEY_LOCAL_MACHINE, szKey, L"JI Pumpkins (TrueType)", L"JI Pumpkins.ttf");
	MessageBox(hWndMain, L"폰트를 설치 하였습니다.", L"알림", MB_OK);
}

void UnInstallFont()
{
	TCHAR FontPath[MAX_PATH];

	GetWindowsDirectory(FontPath, MAX_PATH);
	wcscat(FontPath, L"\\Fonts\\JI Pumpkins.ttf");

	if (RemoveFontResource(FontPath) == FALSE)
	{
		MessageBox(hWndMain, L"폰트 해제를 실패하였습니다.", L"알림", MB_OK);
		InvalidateRect(hWndMain, NULL, TRUE);
		return;
	}
	SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

	OSVERSIONINFO vi;
	vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	TCHAR szKey[255];
	GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		wcscpy(szKey, L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts");
	}
	else
	{
		wcscpy(szKey, L"Software\\Microsoft\\Windows\\CurrentVersion\\Fonts");
	}

	HKEY key;
	DWORD dwDisp;
	RegCreateKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &dwDisp);
	RegDeleteValue(key, L"JI Pumpkins (TrueType)");
	RegCloseKey(key);

	if (DeleteFile(FontPath) == FALSE)
	{
		MessageBox(hWndMain, L"폰트 파일 삭제를 실패하였습니다.", L"알림", MB_OK);
		return;
	}
	MessageBox(hWndMain, L"폰트를 제거 하였습니다..", L"알림", MB_OK);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HFONT	hFont, hOldFont;
	TCHAR strMes[128] = L"Install Font.";

	switch (iMessage)
	{
	case WM_CREATE:
		hWndMain = hWnd;
		SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
		return 0;
	case WM_LBUTTONDOWN:
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		InstallFont();
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return 0;
	case WM_RBUTTONDOWN:
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		UnInstallFont();
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return 0;
	case WM_FONTCHANGE:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hFont = CreateFont(50, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_ROMAN, L"JI Pumpkins");
		hOldFont = (HFONT)SelectObject(hdc, hFont);
		TextOut(hdc, 10, 50, strMes, wcslen(strMes));
		SelectObject(hdc, hOldFont);
		DeleteObject(hFont);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}