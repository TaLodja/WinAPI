#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <cstdio>
#include <string>
#include "resource.h"

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillEdit(HWND hEdit, CHAR sz_buffer[FILENAME_MAX], CHAR digit);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hOrevInst, LPSTR lpCmdLine, INT nCmdShow)
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
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_SETTEXT, 0, (LPARAM)"0");
	}
	break;
	case WM_COMMAND:
	{
		DOUBLE a = 0;
		DOUBLE b = 0;
		BOOL PressedButtonDigit = FALSE;
		BOOL PressedButtonOperation = FALSE;
		CHAR sz_buffer[FILENAME_MAX];
		CHAR sz_expression[FILENAME_MAX];
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
		HWND hExpression = GetDlgItem(hwnd, IDC_EXPRESSION);
		SendMessage(hEdit, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
		INT len = strlen(sz_buffer);
		//INT iFirst = 0, iSecond, iResult;

		//CONST INT IDdigit[10] = { 1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009 };
		//CONST INT IDtwoOperands[4] = {1010, 1011, 1012, 1013};
		//CONST INT IDoneOperands[6] = {1014, 1015, 1016, 1017, 1018, 1019};
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9 && !PressedButtonOperation)
		{
			if (!PressedButtonOperation && !PressedButtonDigit)
			{
				CHAR digit[2] = {};
				_itoa(LOWORD(wParam) - IDC_BUTTON_0, digit, 10);
				INT i = 0;
				while (i < len)
				{
					if (sz_buffer[i] == '0' && !strchr(sz_buffer, '.'))
					{
						sz_buffer[i] = sz_buffer[i + 1];
						len--;
						i++;
					}
					else break;
				}
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)strcat(sz_buffer, digit));
				SendMessage(hExpression, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			PressedButtonDigit = TRUE;
			}
			if (PressedButtonOperation && !PressedButtonDigit)
			{
				strcpy(sz_buffer, "0");
			}
		}
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_POINT:
		{
			if (strchr(sz_buffer, '.')) break;
			if (sz_buffer == "0") strcpy(sz_buffer, "0.");
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)strcat(sz_buffer, "."));
			SendMessage(hExpression, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			PressedButtonDigit = TRUE;
		}
		break;
		case IDC_BUTTON_BSP:
		{
			if (len > 1) sz_buffer[--len] = '\0';
			else sz_buffer[0] = '0';
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			SendMessage(hExpression, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDC_BUTTON_CLEAR:
		{
			a = b = 0;
			PressedButtonDigit = FALSE;
			PressedButtonOperation = FALSE;
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"0");
			SendMessage(hExpression, WM_SETTEXT, 0, (LPARAM)"0");
		}
		break;
		case IDC_BUTTON_PLUS:
		{

			if (a == 0) a = atof(sz_buffer);
			if (PressedButtonOperation)
			{
				if (PressedButtonDigit || b == 0 && !PressedButtonDigit) b = atof(sz_buffer);
				a += b;
			}
			PressedButtonOperation = FALSE;
			if (a == 0) strcpy(sz_buffer, "0");
			else sprintf(sz_buffer, "%a", a);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			CHAR sz_operator[4] = " + ";
			SendMessage(hExpression, WM_GETTEXT, FILENAME_MAX, (LPARAM)strcat(sz_buffer, sz_operator));
		}
		break;
		case IDC_BUTTON_EQUAL:
		{

		}
		break;
		}
	}
	break;
	case WM_CLOSE: EndDialog(hwnd, 0);
	}
	return FALSE;
}