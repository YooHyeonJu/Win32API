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

	//const wchar_t* fileData[] = { L"����\r\n\r\n\r\n������\r\n\r\n\r\n�״³������ϴ����췯��\r\n�����β����̾��⸦\r\n�ٻ��̴¹ٶ�����\r\n���±��ο��ߴ�\r\n�����뷡�ϴ¸�������\r\n����׾�°���\r\n����ؾ���\r\n�׸��������־�������\r\n�ɾ�߰ڴ�\r\n���ù㿡�����̹ٶ�����ġ���." };
	const wchar_t* fileData[] = { L"����\r\n\r\n\r\n������\r\n\r\n\r\n�״³������ϴ����췯��\r\n�����β����̾��⸦\r\n�ٻ��̴¹ٶ�����\r\n���±��ο��ߴ�\r\n�����뷡�ϴ¸�������\r\n����׾�°���\r\n����ؾ���\r\n�׸��������־�������\r\n�ɾ�߰ڴ�\r\n���ù㿡�����̹ٶ�����ġ���." };
	//TCHAR fileData[] = { (TCHAR)"����\r\n\r\n\r\n������\r\n\r\n\r\n�״³������ϴ����췯��\r\n�����β����̾��⸦\r\n�ٻ��̴¹ٶ�����\r\n���±��ο��ߴ�\r\n�����뷡�ϴ¸�������\r\n����׾�°���\r\n����ؾ���\r\n�׸��������־�������\r\n�ɾ�߰ڴ�\r\n���ù㿡�����̹ٶ�����ġ���." };



	DWORD numOfByteWritten = 0;

	switch (iMessage)
	{

	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);

		//���� ����
		hFile = CreateFile((LPCWSTR)L"overture.txt", GENERIC_ALL | FILE_MAP_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		//TextOut(hdc, 20, 40, (LPCWSTR)fileData, _tcslen(fileData));
		TextOut(hdc, 20, 40, (LPCWSTR)fileData, wcslen(*fileData));
		//���Ͽ� ����
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