#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"MemoryEx";

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
	HANDLE hFile, hMapFile;
	TCHAR*pFile;
	RECT rect;

	//const wchar_t* fileData[] = { L"서시\r\n\r\n\r\n윤동주\r\n\r\n\r\n죽는날까지하늘을우러러\r\n한점부끄럼이없기를\r\n잎새이는바람에도\r\n나는괴로워했다\r\n별을노래하는마음으로\r\n모든죽어가는것을\r\n사랑해야지\r\n그리고나한테주어진길을\r\n걸어가야겠다\r\n오늘밤에도별이바람에스치운다." };
	const wchar_t* fileData[] = { L"서시\r\n\r\n\r\n윤동주\r\n\r\n\r\n죽는날까지하늘을우러러\r\n한점부끄럼이없기를\r\n잎새이는바람에도\r\n나는괴로워했다\r\n별을노래하는마음으로\r\n모든죽어가는것을\r\n사랑해야지\r\n그리고나한테주어진길을\r\n걸어가야겠다\r\n오늘밤에도별이바람에스치운다." };
	//TCHAR fileData[] = { (TCHAR)"서시\r\n\r\n\r\n윤동주\r\n\r\n\r\n죽는날까지하늘을우러러\r\n한점부끄럼이없기를\r\n잎새이는바람에도\r\n나는괴로워했다\r\n별을노래하는마음으로\r\n모든죽어가는것을\r\n사랑해야지\r\n그리고나한테주어진길을\r\n걸어가야겠다\r\n오늘밤에도별이바람에스치운다." };



	DWORD numOfByteWritten = 0;

	switch (iMessage)
	{

	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);

		//파일 생성
		hFile = CreateFile((LPCWSTR)L"overture.txt", GENERIC_ALL | FILE_MAP_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		//TextOut(hdc, 20, 40, (LPCWSTR)fileData, _tcslen(fileData));
		TextOut(hdc, 20, 40, (LPCWSTR)fileData, wcslen(*fileData));
		//파일에 쓰기
		WriteFile(hFile, fileData, sizeof(fileData), &numOfByteWritten, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, L"File not exist", L"Error", MB_OK);
		}
		else
		{
			hMapFile = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
			pFile = (TCHAR*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, NULL);

			SetRect(&rect, 10, 10, 500, 400);
			DrawText(hdc, pFile, GetFileSize(hFile, NULL), &rect, DT_EXPANDTABS);

			UnmapViewOfFile(pFile);
			CloseHandle(hMapFile);
			CloseHandle(hFile);
		}
		
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}