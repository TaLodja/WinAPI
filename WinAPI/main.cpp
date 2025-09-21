#include <Windows.h>
#include "resource.h"

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR IpCmdLine, INT nCmdShow)
{
	/*MessageBox
	(
		NULL,
		"Привет WinAPI! \nКак Ваше настроение?",
		"Hello",
		MB_YESNOCANCEL | MB_ICONQUESTION | MB_HELP | MB_DEFBUTTON3 | MB_SYSTEMMODAL
	);*/

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, NULL);

	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:		//Эта секция выполняется один раз, нужна для добавления элементов в окно диалога
	{
	HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
	SetFocus(hEditLogin);
	HICON hIcon = LoadIcon(GetModuleHandleA(NULL), MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	}
		break;
	case WM_COMMAND:		//Обрабатывает команды с клавиатуры и мыши
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_COPY:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
			SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDOK:		MessageBox(hwnd, "Была нажата кнопка OK", "Info", MB_OK | MB_ICONINFORMATION); break;
		case IDCANCEL:	EndDialog(hwnd, 0); break;
		}
		break;
	case WM_CLOSE:			//Закрывает окно
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}