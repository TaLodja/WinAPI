#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <cstdio>
#include <string>
#include "resource.h"

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
		CHAR digit[2] = {};
		CHAR sz_expression[FILENAME_MAX];
		INT operation;
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
		HWND hExpression = GetDlgItem(hwnd, IDC_EXPRESSION);

		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)
		{
			if (!PressedButtonDigit && PressedButtonOperation)
				SendMessage(hExpression, WM_SETTEXT, 0, (LPARAM)"");
			SendMessage(hEdit, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
			_itoa(LOWORD(wParam) - IDC_BUTTON_0, digit, 10);
			if (strlen(sz_buffer) == 1 && sz_buffer[0] == '0')
				sz_buffer[0] = digit[0];
			else strcat(sz_buffer, digit);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			SendMessage(hExpression, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			PressedButtonDigit = TRUE;
		}
		//if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_EQUAL)
		//{
		//	operation = LOWORD(wParam);
		//	if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH && !PressedButtonOperation)
		//	{
		//	SendMessage(hEdit, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
		//	a = atof(sz_buffer);
		//	PressedButtonOperation = TRUE;
		//	PressedButtonDigit = FALSE;
		//	}
		//	if (LOWORD(wParam) == IDC_BUTTON_EQUAL && PressedButtonOperation)
		//	{
		//		DOUBLE buffer = a;
		//		b = atof(sz_buffer);
		//		switch (operation)
		//		{
		//		case IDC_BUTTON_PLUS:
		//		{
		//			sprintf(sz_expression, "%g + %g", buffer, b);
		//			a += b;
		//		}
		//		break;
		//		case IDC_BUTTON_MINUS:
		//		{
		//			sprintf(sz_expression, "%g - %g", buffer, b);
		//			a -= b;
		//		}
		//		break;
		//		case IDC_BUTTON_ASTER:
		//		{
		//			sprintf(sz_expression, "%g * %g", buffer, b);
		//			a *= b;
		//		}
		//		break;
		//		case IDC_BUTTON_SLASH:
		//		{
		//			sprintf(sz_expression, "%g / %g", buffer, b);
		//			if (b != 0) a /= b;
		//			else strcpy(sz_buffer, "Деление на 0!");
		//		}
		//		break;
		//		}
		//		sprintf(sz_buffer, "%g", a);
		//		SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		//		//SendMessage(hExpression, WM_SETTEXT, 0, (LPARAM)sz_expression);
		//		PressedButtonOperation = FALSE;
		//		PressedButtonDigit = FALSE;
		//	}
		//}
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_POINT:
		{
			SendMessage(hEdit, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
			if (!strchr(sz_buffer, '.')) SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)strcat(sz_buffer, "."));
			else break;
			SendMessage(hExpression, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDC_BUTTON_PLUS:
		{
			SendMessage(hEdit, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
			a = atof(sz_buffer);
		}
		break;
		case IDC_BUTTON_BIN_MINUS:
		{
			SendMessage(hEdit, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
			a = -atof(sz_buffer);
			sprintf(sz_buffer, "%g", a);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			SendMessage(hExpression, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		case IDC_BUTTON_REVERSE:
		{
			SendMessage(hEdit, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
			a = atof(sz_buffer);
			b = 1 / a;
			sprintf(sz_buffer, "%g", b);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			sprintf(sz_expression, "1/%g", a);
			SendMessage(hExpression, WM_SETTEXT, 0, (LPARAM)sz_expression);
		}
		break;
		case IDC_BUTTON_POW:
		{
			SendMessage(hEdit, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
			a = atof(sz_buffer);
			b = a * a;
			sprintf(sz_buffer, "%g", b);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			sprintf(sz_expression, "%g^2", a);
			SendMessage(hExpression, WM_SETTEXT, 0, (LPARAM)sz_expression);
		}
		break;
		case IDC_BUTTON_SQRT:
		{
			SendMessage(hEdit, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
			a = atof(sz_buffer);
			if (a >= 0)
			{
				b = sqrt(a);
				sprintf(sz_buffer, "%g", b);
			}
			else strcpy(sz_buffer, "Error!");
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			sprintf(sz_expression, "sqrt(%g)", a);
			SendMessage(hExpression, WM_SETTEXT, 0, (LPARAM)sz_expression);
		}
		break;
		case IDC_BUTTON_BSP:
		{
			SendMessage(hEdit, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
			INT len = strlen(sz_buffer);
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
		}
	}
	break;
	case WM_CLOSE: EndDialog(hwnd, 0);
	}
	return FALSE;
}