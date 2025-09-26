#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <cstdio>
#include "resource.h"

CONST CHAR* VALUES[] = { "This", "is", "my", "First", "List", "Box" };
CHAR ChangeElement[256] = {};
CHAR AddElement[256] = {};

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcAdd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcChange(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR IpCmdLine, INT nCmdShow)
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
		HWND hList = GetDlgItem(hwnd, IDC_LIST);
		for (int i = 0; i < sizeof(VALUES) / sizeof(VALUES[0]); i++)
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)VALUES[i]);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_Add:
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST);
			DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_ADD), NULL, (DLGPROC)DlgProcAdd, 0);
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)AddElement);
		}
		break;
		case IDC_LIST:
		{
			if (HIWORD(wParam) == LBN_DBLCLK)
			{
				HWND hList = GetDlgItem(hwnd, IDC_LIST);
				INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
				SendMessage(hList, LB_DELETESTRING, i, 0);
				DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_CHANGE), NULL, (DLGPROC)DlgProcChange, 0);
				SendMessage(hList, LB_INSERTSTRING, i, (LPARAM)ChangeElement);
			}
		}
			break;
		case IDC_DEL:
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
			SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer);
			SendMessage(hList, LB_DELETESTRING, i, 0);
			CHAR sz_message[SIZE];
			sprintf(sz_message, "Вы удалили элемент №%i со значением '%s'", i, sz_buffer);
			MessageBox(hwnd, sz_message, "Info", MB_OK);
		}
		break;
		case IDOK:
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
			SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer);
			CHAR sz_message[SIZE];
			sprintf(sz_message, "Вы выбрали вариант №%i со значением '%s'", i, sz_buffer);
			if (i != -1)
				MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
			else MessageBox(hwnd, "Выберите элемент списка", "Info", MB_OK | MB_ICONINFORMATION);
		}
		break;
		case IDCANCEL:
			EndDialog(hwnd, 0); break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}

BOOL CALLBACK DlgProcAdd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hAddElement = GetDlgItem(hwnd, IDC_Add);
		SetFocus(hAddElement);
	}
	break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			HWND hAddElement = GetDlgItem(hwnd, IDC_Add);
			SendMessage(hAddElement, WM_GETTEXT, 256, (LPARAM)AddElement);
			EndDialog(hwnd, 0);
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}

BOOL CALLBACK DlgProcChange(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hChangeElement = GetDlgItem(hwnd, IDC_EDIT);
		SetFocus(hChangeElement);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND hChangeElement = GetDlgItem(hwnd, IDC_EDIT);
			SendMessage(hChangeElement, WM_GETTEXT, 256, (LPARAM)ChangeElement);
			EndDialog(hwnd, 0);
		}
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0); break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}