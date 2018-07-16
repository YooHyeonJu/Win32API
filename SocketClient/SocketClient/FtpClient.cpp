#include <windows.h>
#include <wininet.h>
#include <commctrl.h>
#include "resource.h"

HINTERNET hInternet, hFtp;
HWND hState, hConnect, hDisconnect, hUpload, hList1;
HWND hUrl, hUserID, hPass;

char g_Url[255] = "ftp.microsoft.com";
char g_User[128] = "anonymous";
char g_Password[128] = "1234";
HINSTANCE g_hInst;
HWND g_hDlg;

BOOL CALLBACK   DialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK   FtpConnectProc(HWND, UINT, WPARAM, LPARAM);
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int
	nCmdShow)
{
	DialogBox(hInstance, (LPSTR)IDD_DIALOG1, HWND_DESKTOP, DialogProc);
	g_hInst = hInstance;
}

BOOL FtpConnect()
{
	hInternet = InternetOpen("FtpClient", INTERNET_OPEN_TYPE_PRECONFIG,
		NULL, NULL, 0);
	if (hInternet == NULL)
	{
		return FALSE;
	}

	hFtp = InternetConnect(hInternet, g_Url, INTERNET_DEFAULT_FTP_PORT,
		g_User, g_Password, INTERNET_SERVICE_FTP, 0, 0);
	if (hFtp == NULL)
	{
		InternetCloseHandle(hInternet);
		return FALSE;
	}

	return TRUE;
}

// FTP 연결을 끊는다.
void FtpDisconnect()
{
	InternetCloseHandle(hFtp);
	InternetCloseHandle(hInternet);
	hFtp = NULL;
	hInternet = NULL;
	SendMessage(hState, LB_ADDSTRING, 0, (LPARAM)"서버에 접속 해제 하였습니다..");
}

// 파일을 리스트 뷰에 출력한다.
void AddFileToList(WIN32_FIND_DATA wfd)
{
	LVITEM LI;
	int idx;
	char szTmp[50];
	SYSTEMTIME st;

	LI.mask = LVIF_TEXT;
	LI.iSubItem = 0;
	LI.iItem = 10000;
	LI.pszText = wfd.cFileName;
	idx = ListView_InsertItem(hList1, &LI);

	if (strcmp(wfd.cFileName, "..") != 0)
	{
		itoa(wfd.nFileSizeLow, szTmp, 10);
		ListView_SetItemText(hList1, idx, 1, szTmp);
		FileTimeToSystemTime(&wfd.ftLastWriteTime, &st);
		wsprintf(szTmp, "%d년 %d월 %d일 %d시 %d분", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
		ListView_SetItemText(hList1, idx, 2, szTmp);
	}
}

void FtpListFiles()
{
	WIN32_FIND_DATA wfd;
	BOOL bResult = TRUE;
	HINTERNET hFind;

	if (hFtp == NULL)
		return;

	ListView_DeleteAllItems(hList1);
	strcpy(wfd.cFileName, "..");
	AddFileToList(wfd);

	hFind = FtpFindFirstFile(hFtp, "*.*", &wfd, 0, 0);
	if (hFind != NULL)
	{
		AddFileToList(wfd);
		while (bResult)
		{
			bResult = InternetFindNextFile(hFind, &wfd);
			if (bResult == TRUE)
			{
				AddFileToList(wfd);
			}
		}
		InternetCloseHandle(hFind);
	}
}

BOOL FtpDownLoad(char *Path)
{
	char NewFile[MAX_PATH];

	memset(NewFile, 0, MAX_PATH);
	GetCurrentDirectory(MAX_PATH, NewFile);
	strcat(NewFile, "\\");
	strcat(NewFile, Path);

	return FtpGetFile(hFtp, Path, NewFile, FALSE, FILE_ATTRIBUTE_NORMAL,
		FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 0);
}

void OnDblClick(int iItem)
{
	LVITEM LI;
	char Path[MAX_PATH];
	BOOL Result;

	if ((hFtp == NULL) || (iItem == -1))
		return;

	if (iItem == 0)
	{
		if (FtpSetCurrentDirectory(hFtp, "..") == TRUE)
			FtpListFiles();
		return;
	}

	LI.iItem = iItem;
	LI.iSubItem = 0;
	LI.pszText = Path;
	LI.cchTextMax = 255;
	LI.mask = LVIF_TEXT;
	ListView_GetItem(hList1, &LI);

	if (FtpSetCurrentDirectory(hFtp, Path) == TRUE)
	{
		FtpListFiles();
		return;
	}

	Result = FtpDownLoad(Path);

	if (Result == TRUE)
	{
		SendMessage(hState, LB_ADDSTRING, 0, (LPARAM)"다운로드 받았습니다.");
	}
	else
	{
		SendMessage(hState, LB_ADDSTRING, 0, (LPARAM)"다운로드 실패했습니다.");
	}

}

BOOL FtpUpLoad(char *Path)
{
	char ext[_MAX_EXT];
	char Remote[MAX_PATH];

	_splitpath(Path, NULL, NULL, Remote, ext);
	strcat(Remote, ext);
	return FtpPutFile(hFtp, Path, Remote,
		FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 0);
}

//서버에 업로드 하는 루틴
void OnUpLoad()
{
	OPENFILENAME OFN;
	char lpstrFile[MAX_PATH] = "";

	if (hFtp == NULL)
	{
		SendMessage(hState, LB_ADDSTRING, 0, (LPARAM)"FTP 서버에 연결되지 않았습니다.");
		return;
	}

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = g_hDlg;
	OFN.lpstrFilter = "Every File(*.*)\0*.*\0";
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 256;
	OFN.lpstrInitialDir = "c:\\";
	if (GetOpenFileName(&OFN) != 0)
	{
		if (FtpUpLoad(OFN.lpstrFile) == TRUE)
		{
			SendMessage(hState, LB_ADDSTRING, 0, (LPARAM)"FTP 서버에 업로드 하였습니다.");
			FtpListFiles();
		}
		else
		{
			SendMessage(hState, LB_ADDSTRING, 0, (LPARAM)"FTP 서버에 업로드 실패하였습니다.");
		}
	}
}

//접속 대화상자 프로시져
BOOL CALLBACK   FtpConnectProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hDlg, IDC_URL, g_Url);
		SetDlgItemText(hDlg, IDC_ID, g_User);
		SetDlgItemText(hDlg, IDC_PASS, g_Password);
	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			GetDlgItemText(hDlg, IDC_URL, g_Url, 255);
			GetDlgItemText(hDlg, IDC_ID, g_User, 128);
			GetDlgItemText(hDlg, IDC_PASS, g_Password, 128);
			EndDialog(hDlg, 0);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
		return TRUE;
	}
	return FALSE;
}

BOOL  CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	LV_COLUMN COL;
	LPNMHDR hdr;
	LPNMITEMACTIVATE nia;

	switch (message)
	{
	case WM_INITDIALOG:
		hState = GetDlgItem(hDlg, IDC_STATE);
		hConnect = GetDlgItem(hDlg, IDC_CONNECT);
		hDisconnect = GetDlgItem(hDlg, IDC_DISCONNECT);
		hUpload = GetDlgItem(hDlg, IDC_UPLOAD);
		hList1 = GetDlgItem(hDlg, IDC_LIST1);

		COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		COL.fmt = LVCFMT_LEFT;
		COL.cx = 200;
		COL.pszText = (LPSTR)"File";
		COL.iSubItem = 0;
		ListView_InsertColumn(hList1, 0, &COL);

		COL.cx = 50;
		COL.pszText = (LPSTR)"Size";
		COL.iSubItem = 1;
		ListView_InsertColumn(hList1, 1, &COL);

		COL.cx = 300;
		COL.pszText = (LPSTR)"Date";
		COL.iSubItem = 2;
		ListView_InsertColumn(hList1, 2, &COL);
		g_hDlg = hDlg;
		return TRUE;
	case WM_NOTIFY:
		hdr = (LPNMHDR)lParam;
		nia = (LPNMITEMACTIVATE)lParam;

		if (hdr->hwndFrom == hList1)
		{
			switch (hdr->code)
			{
			case NM_DBLCLK:
				OnDblClick(nia->iItem);
				break;
			}
		}
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case IDC_CONNECT:
			if (DialogBox(g_hInst, MAKEINTRESOURCE(IDD_CONNECT), hDlg, FtpConnectProc) == IDCANCEL)
				return FALSE;
			if (FtpConnect())
				SendMessage(hState, LB_ADDSTRING, 0, (LPARAM)"서버에 접속하였습니다.");
			else
			{
				SendMessage(hState, LB_ADDSTRING, 0, (LPARAM)"서버 접속에 실패하였습니다.");
				return FALSE;
			}

			FtpListFiles();
			return TRUE;
		case IDC_DISCONNECT:
			FtpDisconnect();
			return TRUE;
		case IDC_UPLOAD:
			OnUpLoad();
			return TRUE;
		case IDOK:
			EndDialog(hDlg, 0);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}
