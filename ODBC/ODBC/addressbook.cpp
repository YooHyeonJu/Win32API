#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <commctrl.h>
#include "resource.h"

HINSTANCE g_hInst;
HWND	  g_hWnd;
const char* lpszClass = "addressbook";

SQLHENV	hEnv;
SQLHDBC	hOdbc;
SQLHSTMT	hStmt;

HWND	hListCtrl, hEditNumber, hEditName, hEditMobile, hEditAddress;
HWND g_hDlg;
LVCOLUMN column;
LVITEM listItem;
LPNMHDR	pnmh;
LPNM_LISTVIEW pInfo;
int idx;
char szTemp[256];

SQLCHAR		szNumber[MAX_PATH];
SQLCHAR		szName[MAX_PATH];
SQLCHAR		szOldName[MAX_PATH], szNewName[MAX_PATH];
SQLCHAR		szMobile[MAX_PATH];
SQLCHAR		szAddress[MAX_PATH];
char szSQL[256];
SQLINTEGER lNumber, lName, lMobile, lAddress;

void ODBCConnect();
void DBDisConnect();
void ListCtrlView(HWND hListCtrl);

BOOL CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	g_hInst = hInstance;
	ODBCConnect();	//데이터 소스에 연결하기
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ADDRESSBOOK), HWND_DESKTOP, DlgProc);
	DBDisConnect();	//데이터 소스 연결 해제
}

//데이터 소스에 연결하기
void ODBCConnect()
{
	//환경 핸들 할당
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
		return;
	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
		return;

	//연결 핸들 할당
	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hOdbc) != SQL_SUCCESS)
		return;

	SQLConnect(hOdbc, (SQLCHAR*)"UserInfo", SQL_NTS, (SQLCHAR*)"Admin", SQL_NTS, (SQLCHAR*)"", SQL_NTS);

	if (SQLAllocHandle(SQL_HANDLE_STMT, hOdbc, &hStmt) != SQL_SUCCESS)
		return;
}

//데이터 명령 실행하기
BOOL ODBExecuteSQL()
{
	ListView_DeleteAllItems(hListCtrl);
	SQLBindCol(hStmt, 1, SQL_C_CHAR, &szName, sizeof(szName), &lName);
	SQLBindCol(hStmt, 2, SQL_C_CHAR, &szNumber, sizeof(szNumber), &lNumber);
	SQLBindCol(hStmt, 3, SQL_C_CHAR, &szMobile, sizeof(szMobile), &lMobile);
	SQLBindCol(hStmt, 4, SQL_C_CHAR, &szAddress, sizeof(szAddress), &lAddress);

	// SQL문을 실행한다.
	if (SQLExecDirect(hStmt, (SQLCHAR*)"select name, idnumber,mobile, address from tblUserInfo", SQL_NTS) != SQL_SUCCESS)
	{
		return FALSE;
	}

	while (SQLFetch(hStmt) != SQL_NO_DATA)
	{
		listItem.mask = LVIF_TEXT;
		listItem.state = 0;
		listItem.stateMask = 0;
		listItem.iSubItem = 0;
		listItem.iItem = 0;
		listItem.pszText = (LPSTR)szName;
		ListView_InsertItem(hListCtrl, &listItem);
		ListView_SetItemText(hListCtrl, 0, 1, (LPSTR)szNumber);
		ListView_SetItemText(hListCtrl, 0, 2, (LPSTR)szMobile);
		ListView_SetItemText(hListCtrl, 0, 3, (LPSTR)szAddress);
	}

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

void ListCtrlView(HWND hListCtrl)
{
	const char *Columnlist[4] = { "이름", "주민번호", "휴대폰", "주소" };
	int nWidth[4] = { 50, 100, 150, 150 };
	LPNMHDR	pnmh;
	int nGetSel;

	for (int i = 0; i < 4; i++)
	{
		column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		column.fmt = LVCFMT_LEFT;
		column.cx = nWidth[i];
		column.pszText = (LPSTR)Columnlist[i];
		column.iSubItem = 0;
		ListView_InsertColumn(hListCtrl, i, &column);
	}
}

void InsertRecord()
{
	GetWindowText(hEditNumber, (LPTSTR)szNumber, 260);
	GetWindowText(hEditName, (LPTSTR)szName, 260);
	GetWindowText(hEditMobile, (LPTSTR)szMobile, 260);
	GetWindowText(hEditAddress, (LPTSTR)szAddress, 260);

	wsprintf(szSQL, "Insert into tblUserInfo (name, idnumber, mobile, address) VALUES ('%s','%s','%s','%s')", szName, szNumber, szMobile, szAddress);

	if (SQLExecDirect(hStmt, (SQLCHAR*)szSQL, SQL_NTS) != SQL_SUCCESS)
	{
		return;
	}

	if (hStmt)
		SQLCloseCursor(hStmt);
	ODBExecuteSQL();
}

//데이터 읽어오기
BOOL ReadData(LPNM_LISTVIEW pInfo)
{
	char Key[256];

	ListView_GetItemText(hListCtrl, pInfo->iItem, pInfo->iSubItem, szTemp, MAX_PATH);
	idx = pInfo->iItem;
	if (idx == -1)
		return FALSE;

	wsprintf(szSQL, "select name, idnumber, mobile, address from tblUserInfo where name='%s'", szTemp);
	SQLBindCol(hStmt, 1, SQL_C_CHAR, &szName, sizeof(szName), &lName);
	SQLBindCol(hStmt, 2, SQL_C_CHAR, &szNumber, sizeof(szNumber), &lNumber);
	SQLBindCol(hStmt, 3, SQL_C_CHAR, &szMobile, sizeof(szMobile), &lMobile);
	SQLBindCol(hStmt, 4, SQL_C_CHAR, &szAddress, sizeof(szAddress), &lAddress);

	if (SQLExecDirect(hStmt, (SQLCHAR *)szSQL, SQL_NTS) != SQL_SUCCESS)
	{
		return FALSE;
	}

	if (SQLFetch(hStmt) != SQL_NO_DATA)
	{
		SetWindowText(hEditName, (LPCTSTR)szName);
		SetWindowText(hEditNumber, (LPCTSTR)szNumber);
		SetWindowText(hEditMobile, (LPCTSTR)szMobile);
		SetWindowText(hEditAddress, (LPCTSTR)szAddress);
	}

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

void DeleteRecord()
{
	char Key[256], szSQL[256];
	wsprintf(szSQL, "Delete from tblUserInfo where name='%s'", szTemp);

	if (SQLExecDirect(hStmt, (SQLCHAR *)szSQL, SQL_NTS) != SQL_SUCCESS)
		return;

	if (hStmt) SQLCloseCursor(hStmt);
	ODBExecuteSQL();
}

void UpdateRecord()
{
	GetWindowText(hEditName, (LPTSTR)szNewName, 20);
	GetWindowText(hEditNumber, (LPTSTR)szNumber, 20);
	GetWindowText(hEditMobile, (LPTSTR)szMobile, 20);
	GetWindowText(hEditAddress, (LPTSTR)szAddress, 20);

	SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 21, 0, szNewName, 0, &lName);
	SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_C_CHAR, 21, 0, &szNumber, 0, &lNumber);
	SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_C_CHAR, 21, 0, &szMobile, 0, &lMobile);
	SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 21, 0, szAddress, 0, &lAddress);
	SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 21, 0, szTemp, 0, &lName);

	wsprintf(szSQL, "Update tblUserInfo set name=?, idnumber=?, mobile=?, address=? where name=?");
	if (SQLExecDirect(hStmt, (SQLCHAR *)szSQL, SQL_NTS) != SQL_SUCCESS)
		return;

	if (hStmt) SQLCloseCursor(hStmt);
	ODBExecuteSQL();
}

//데이터 소스 연결 해제(대화상자 종료될 때)
void DBDisConnect()
{
	// 뒷정리
	if (hStmt) SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	if (hOdbc) SQLDisconnect(hOdbc);
	if (hOdbc) SQLFreeHandle(SQL_HANDLE_DBC, hOdbc);
	if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	DWORD style;

	switch (iMessage)
	{
	case WM_INITDIALOG:
		g_hDlg = hDlg;
		hListCtrl = GetDlgItem(g_hDlg, IDC_LIST1);
		hEditNumber = GetDlgItem(g_hDlg, IDC_EDIT_NUM);
		hEditName = GetDlgItem(g_hDlg, IDC_EDIT_NAME);
		hEditMobile = GetDlgItem(g_hDlg, IDC_EDIT_MOBILE);
		hEditAddress = GetDlgItem(g_hDlg, IDC_EDIT_ADDRESS);
		ListCtrlView(hListCtrl);
		ODBExecuteSQL();
		style = GetWindowLong(hListCtrl, GWL_STYLE);
		style &= ~(LVS_TYPEMASK);
		style = style | LVS_SMALLICON;
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_INSERT:
			InsertRecord();
			return TRUE;
		case IDC_DELETE:
			DeleteRecord();
			return TRUE;
		case IDC_UPDATE:
			UpdateRecord();
			return TRUE;
		case IDC_CLOSE:
			EndDialog(g_hDlg, TRUE);
			return TRUE;
		}
		return FALSE;
	case WM_NOTIFY:
		if (wParam == IDC_LIST1)
		{
			pnmh = (LPNMHDR)lParam;
			switch (pnmh->code)
			{
			case LVN_ITEMCHANGED:
				pInfo = (LPNM_LISTVIEW)lParam;
				ReadData(pInfo);
				break;
			}
		}
	}

	return FALSE;
}

