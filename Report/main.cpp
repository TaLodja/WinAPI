#include <Windows.h>
#include <Commctrl.h>
#include <cstdio>
#include "resource.h"

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SetFocus(GetDlgItem(hwnd, IDC_SURNAME));
		HICON hIcon = LoadIcon(GetModuleHandleA(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_GEN:
		{
			CHAR* Characteristics[10] = {};
			HWND hList = GetDlgItem(hwnd, IDC_LIST);
			HWND FullName[3] = {};
			FullName[0] = GetDlgItem(hwnd, IDC_SURNAME);
			FullName[1] = GetDlgItem(hwnd, IDC_NAME);
			FullName[2] = GetDlgItem(hwnd, IDC_PATR);
			CHAR sz_buffer[FILENAME_MAX] = {};
			CHAR sz_descript[FILENAME_MAX] = {};
			for (int i = 0; i < 3; i++)
			{
				SendMessage(FullName[i], WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
				if (sz_buffer != "")
				{
					Characteristics[i] = sz_buffer;
					SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)Characteristics[i]);
				}
			}
			HWND hCheck = GetDlgItem(hwnd, IDC_CHECK1);
			if (SendMessage(hCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
			SendMessage(hCheck, BCM_GETNOTE, FILENAME_MAX, (LPARAM)sz_descript);
			Characteristics[4] = sz_descript;
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)Characteristics[4]);
			}
		}
		break;
		case IDOK:
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST);
			if (SendMessage(hList, LB_GETCOUNT, 0, 0) < 3)
				MessageBox(hwnd, "Вы не сгенерировали отзыв", "Info", MB_OK | MB_ICONINFORMATION);
			else MessageBox(hwnd, "Отзыв о студенте составлен", "Info", MB_OK | MB_ICONINFORMATION);
		}
		break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
		}
	}
	break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}