#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <cstdio>
#include "resource.h"

CONST CHAR* g_ITEMS[] = { "This", "is", "my", "First", "List", "Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcAdd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//hInstance - экземпляр запущенного *.exe-файла нашей программы
	//hPrevInst - предыдущий запущенный экземпляр нашей программы.
	//		Этот параметр использовался только в Windows 98/
	//		Во всех последующих ОС Windows он не используется.
	//lpCmdLine - командная строка, с которой была запущена программа.
	//nCmdShow - режим отображения окна: Развернуто на весь экран, Свернуто в окно, Свернуто на панель задач...
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
		for (int i = 0; i < sizeof(g_ITEMS) / sizeof(g_ITEMS[0]); i++)
			SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)g_ITEMS[i]);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_ADD:
		{
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, (DLGPROC)DlgProcAdd, 0);
		}
		break;
		case IDOK:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			CHAR sz_message[SIZE] = {};
			HWND hListBox = GetDlgItem(hwnd, IDC_LIST);
			INT i = SendMessage(hListBox, LB_GETCURSEL, 0, (LPARAM)hListBox) + 1;	//Get Current Selection
			SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)sz_buffer);
			sprintf(sz_message, "Вы выбрали пункт №%i со значением \"%s\"", i, sz_buffer);
			MessageBox(hwnd, sz_message, "Info", MB_OK | MB_ICONINFORMATION);
		}
		break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
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
		SetFocus(GetDlgItem(hwnd, IDC_EDIT_ITEM));
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND hEditItem = GetDlgItem(hwnd, IDC_EDIT_ITEM);
			CHAR sz_buffer[FILENAME_MAX] = {};
			SendMessage(hEditItem, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
			HWND hListBox = GetDlgItem(GetParent(hwnd), IDC_LIST);
			if (SendMessage(hListBox, LB_FINDSTRINGEXACT, 0, (LPARAM)sz_buffer) == LB_ERR)
				SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
			else
			{
				MessageBox(hwnd, "Такой пункт уже есть в списке, введите другое значение", "Опаньки", MB_OK | MB_ICONINFORMATION);
				break;
			}
		}
		case IDCANCEL:
			EndDialog(hwnd, 0);
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}