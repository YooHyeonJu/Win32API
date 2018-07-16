#include <windows.h>
#include <tchar.h>

//#define WS_OVERLAPPEDWINDOW WS_CAPTION | WS_SYSMENU
//WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_BORDER | WS_CHILD | WS_POPUP 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
const wchar_t* lpszClass = L"ApiBase";


/////1.Ŭ���� ����

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;		//Ŭ���� ���� ����Ʈ ��
	WndClass.cbWndExtra = 0;		//������ ���� ����Ʈ ��
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//������ ����
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);		//���α׷����� ����� Ŀ��
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//����� ������
	WndClass.hInstance = hInstance;				//�ν��Ͻ� �ڵ�
	WndClass.lpfnWndProc = (WNDPROC)WndProc;		//���ν��� �Լ���
	WndClass.lpszClassName = lpszClass;			//����ü�� ������� Ŭ������
	WndClass.lpszMenuName = NULL;				//���α׷����� ����� �޴�
	WndClass.style = CS_HREDRAW | CS_VREDRAW;		//������ ��Ÿ��

	RegisterClass(&WndClass);	//Ŭ������ ���


								////2.Ŭ���� ��ü ����

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);

	
	////3.������ ��ü ȭ�鿡 ����

	ShowWindow(hWnd, nCmdShow);

	DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL;
	SetWindowLong(hWnd, GWL_STYLE, dwStyle);
	SendMessage(hWnd, WM_NCPAINT, 1, 0);


	////4.�޽��� ���� ������ -> ������ ���ν��� �Լ��� �޽����� �����Ѵ�.

	while (GetMessage(&Message, 0, 0, 0)) {	//������ ������ �ݺ����� ����. (WM-QUIT�̸� GetMessage �Լ��� ���� ��ȯ)
		TranslateMessage(&Message);	//���� �Է��� ó���ϴ� �Լ�
		DispatchMessage(&Message);	//GetMessage �Լ��κ��� ���޵� �޽����� ������ ���ν����� ������ �Լ�
									//	(�޽����� ������ ó�� �������� ��ȯ���� �ʴ´�.)
	}
	return Message.wParam;
}

////5.�޽��� ó��

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	HDC hdc;
	PAINTSTRUCT ps;
	LONG dwStyle;

	/////////////������ ��Ÿ�� ����
	TCHAR strTitle[] = L"������ ��Ÿ�� ����";


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