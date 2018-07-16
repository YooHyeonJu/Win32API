#include <windows.h>
#include <sql.h>
#include <sqlext.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND	  g_hWnd;
const char* lpszClass = "odbcbase";

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
	g_hWnd = hWnd;

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

void OutputODBCTest()
{
	//핸들
	SQLHENV		hEnv;
	SQLHDBC		hODbc;
	SQLHSTMT		hStmt;

	//결과
	SQLCHAR		szNumber[128];
	SQLCHAR		szName[128];
	SQLCHAR		szMobile[128];
	SQLCHAR		szAddress[128];

	SQLINTEGER	lNumber, lName, lMobile, lAddress;

	//출력
	HDC hdc;
	char strTemp[MAX_PATH];
	int	 nTap[3] = { 200, 200, 400 };
	int	 nEnter = 1;

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);

	SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hODbc);
	SQLConnect(hODbc, (SQLCHAR*)"UserInfo", SQL_NTS, (SQLCHAR*)"Admin", SQL_NTS, (SQLCHAR*)"", SQL_NTS);

	//명령핸들 할당
	SQLAllocHandle(SQL_HANDLE_STMT, hODbc, &hStmt);

	SQLBindCol(hStmt, 1, SQL_C_CHAR, &szName, sizeof(szName), &lName);
	SQLBindCol(hStmt, 2, SQL_C_CHAR, &szNumber, sizeof(szNumber), &lNumber);
	SQLBindCol(hStmt, 3, SQL_C_CHAR, &szMobile, sizeof(szMobile), &lMobile);
	SQLBindCol(hStmt, 4, SQL_C_CHAR, &szAddress, sizeof(szAddress), &lAddress);

	//tblUserInfo 테이블에서 각 필드를 가져온다.
	SQLExecDirect(hStmt, (SQLCHAR*)"select name, idnumber, mobile, address from tblUserInfo", SQL_NTS);

	hdc = GetDC(g_hWnd);
	wsprintf(strTemp, "이름\t주민번호\t휴대폰\t주소");
	TabbedTextOut(hdc, 10, 20 * nEnter, strTemp, strlen(strTemp), 3, nTap, 0);
	nEnter++;

	while (SQLFetch(hStmt) != SQL_NO_DATA)
	{
		wsprintf(strTemp, "%s\t%s\t%s\t%s", szName, szNumber, szMobile, szAddress);
		TabbedTextOut(hdc, 10, 20 * nEnter, strTemp, strlen(strTemp), 3, nTap, 0);
		nEnter++;
	}

	//정리
	SQLCloseCursor(hStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	SQLDisconnect(hODbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hODbc);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		OutputODBCTest();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}