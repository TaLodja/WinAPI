#define _CRT_SECURE_NO_WARNINGS
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
			HWND hList = GetDlgItem(hwnd, IDC_LIST);
			CHAR sz_buffer[FILENAME_MAX] = {};
			for (int i = 1001; i <= 1003; i++)
			{
				CHAR sz_message[FILENAME_MAX] = {};
				SendMessage(GetDlgItem(hwnd, i), WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
				switch (i)
				{
				case 1001:
				{
					sprintf(sz_message, "Фамилия: %s", sz_buffer);
				}
				break;
				case 1002:
					sprintf(sz_message, "Имя: %s", sz_buffer);
					break;
				case 1003:
					sprintf(sz_message, "Отчество: %s", sz_buffer);
				}
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)sz_message);
			}
			for (int i = 1004; i <= 1010; i++)
			{
				CHAR sz_message[FILENAME_MAX] = {};
				HWND hCheck = GetDlgItem(hwnd, i);
				SendMessage(hCheck, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
				if (SendMessage(hCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)
				{
					if (i <= 1007) sprintf(sz_message, "+ %s", sz_buffer);
					else sprintf(sz_message, "- %s", sz_buffer);
					SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)sz_message);
				}
			}
		}
		break;
		case IDC_BUTTON_CLEAR:
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST);
			INT Count = SendMessage(hList, LB_GETCOUNT, 0, 0);
			for (int i = Count; i > 0; i--)
				SendMessage(hList, LB_DELETESTRING, i-1, 0);
		}
		break;
		case IDOK:
		{
			CONST CHAR* l_contain[] = { "Фамилия", "Имя" };
			HWND hList = GetDlgItem(hwnd, IDC_LIST);
			if (MessageBox(hwnd, "Отзыв о студенте составлен?", "Info", MB_YESNO | MB_ICONQUESTION) == IDYES) EndDialog(hwnd, 0);
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