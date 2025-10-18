#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <cstdio>
#include "resource.h"

CONST CHAR g_sz_CLASS_NAME[] = "Calc_SPU_411";

CONST INT g_i_BUTTON_SIZE = 80;		//Размер кнопки
CONST INT g_i_INTERVAL = 2;			//Расстояние между кнопками
CONST INT g_i_BUTTON_DOUBLE_SIZE = g_i_BUTTON_SIZE * 2 + g_i_INTERVAL;

CONST INT g_i_START_X = 10;
CONST INT g_i_START_Y = 10;

CONST INT g_i_SCREEN_WIDTH = (g_i_BUTTON_SIZE + g_i_INTERVAL) * 5 - g_i_INTERVAL;
CONST INT g_i_SCREEN_HEIGHT = g_i_BUTTON_SIZE;

CONST INT g_i_BUTTON_START_X = g_i_START_X;
CONST INT g_i_BUTTON_START_Y = g_i_START_Y + g_i_SCREEN_HEIGHT + g_i_INTERVAL * 4;

CONST CHAR g_OPERATIONS[] = "+-*/";
CONST INT g_i_OPERATIONS_START_X = g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3;

#define BUTTON_SHIFT_X(shift) g_i_BUTTON_START_X+(g_i_BUTTON_SIZE + g_i_INTERVAL)*(shift)
#define BUTTON_SHIFT_Y(shift) g_i_BUTTON_START_Y+(g_i_BUTTON_SIZE + g_i_INTERVAL)*(shift)

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
DOUBLE arithmetic_operation(WORD operation, DOUBLE a, DOUBLE b);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) Регистрация класса окна
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbWndExtra = 0;
	wClass.cbClsExtra = 0;

	wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wClass.hInstance - hInstance;
	wClass.lpfnWndProc = WndProc;
	wClass.lpszMenuName = NULL;
	wClass.lpszClassName = g_sz_CLASS_NAME;

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	//2) Создание окна
	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_CLASS_NAME,
		g_sz_CLASS_NAME,
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		g_i_SCREEN_WIDTH + 2 * g_i_START_X + 8 * 2,
		g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 4 + 8 * 2 + 32,	//32 pixel - высота заготовка
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//3) Запуск цикла сообщений
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hEdit = CreateWindowEx
		(
			NULL, "Edit", "0",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			10, 10,
			g_i_SCREEN_WIDTH, g_i_SCREEN_HEIGHT,
			hwnd,
			(HMENU)IDC_EDIT,
			GetModuleHandle(NULL),
			NULL
		);
		INT digit = 1;
		CHAR sz_digit[2] = "";
		for (int i = 6; i >= 0; i -= 3)
		{
			for (int j = 0; j < 3; j++)
			{
				sz_digit[0] = digit + '0';
				CreateWindowEx
				(
					NULL, "Button", sz_digit,
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					BUTTON_SHIFT_X(j), BUTTON_SHIFT_Y(i / 3),
					//g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * j,
					//g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * i/3,
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd,
					(HMENU)(IDC_BUTTON_0 + digit),
					GetModuleHandle(NULL),
					NULL
				);
				digit++;
			}
		}
		CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_SHIFT_X(0), BUTTON_SHIFT_Y(3),
			g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_0,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_SHIFT_X(2), BUTTON_SHIFT_Y(3),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_POINT,
			GetModuleHandle(NULL),
			NULL
		);
		CHAR operation[2] = "";
		for (int i = 0; i < 4; i++)
		{
			operation[0] = g_OPERATIONS[i];
			CreateWindowEx
			(
				NULL, "Button", operation,
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				BUTTON_SHIFT_X(3), BUTTON_SHIFT_Y(3 - i),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_PLUS + i),
				GetModuleHandle(NULL),
				NULL
			);
		}
		CreateWindowEx
		(
			NULL, "Button", "<-",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_SHIFT_X(4), BUTTON_SHIFT_Y(0),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_BSP,
			GetModuleHandle(NULL),
			NULL
		);

		CreateWindowEx
		(
			NULL, "Button", "C",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_SHIFT_X(4), BUTTON_SHIFT_Y(1),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_CLR,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "=",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_SHIFT_X(4), BUTTON_SHIFT_Y(2),
			g_i_BUTTON_SIZE, g_i_BUTTON_DOUBLE_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_EQUAL,
			GetModuleHandle(NULL),
			NULL
		);
	}
	break;
	case WM_COMMAND:
	{
		FLOAT a = 0, b = 0;
		CHAR sz_buffer[FILENAME_MAX] = {};
		CHAR sz_digit[2] = {};
		INT operation = 0;
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);

		BOOL input_digit = FALSE;
		BOOL input_operation = FALSE;
		BOOL input_equal = FALSE;

		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)
		{
			if (input_operation || input_equal)
			{
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"0");
				input_operation = input_equal = FALSE;
			}
			SendMessage(hEdit, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
			_itoa(LOWORD(wParam) - IDC_BUTTON_0, sz_digit, 10);
			if (sz_buffer[0] == '0' && strlen(sz_buffer) == 1)
				sz_buffer[0] = sz_digit[0];
			else strcat(sz_buffer, sz_digit);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			input_digit = TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_POINT)
		{
			SendMessage(hEdit, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
			if (!strchr(sz_buffer, '.'))
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)strcat(sz_buffer, "."));
			else break;
		}
		if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			SendMessage(hEdit, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
			operation = LOWORD(wParam);
			if (input_digit && a == 0) a = atof(sz_buffer);
			if (input_digit && a != 0)
			{
				b = atof(sz_buffer);
				a = arithmetic_operation(operation, a, b);
				sprintf(sz_buffer, "%f", a);
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			}
			input_operation = TRUE;
			input_digit = input_equal = FALSE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{
			SendMessage(hEdit, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
			if (input_digit) b = atof(sz_buffer);
			a = arithmetic_operation(operation, a, b);
			sprintf(sz_buffer, "%f", a);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			input_equal = TRUE;
			input_digit = input_operation = FALSE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_CLR)
		{
			a = b = 0;
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"0");
		}
		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			SendMessage(hEdit, WM_GETTEXT, FILENAME_MAX, (LPARAM)sz_buffer);
			INT len = strlen(sz_buffer);
			if (len > 1) sz_buffer[--len] = '\0';
			else sz_buffer[0] = '0';
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		SendMessage(hwnd, WM_DESTROY, 0, 0);
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}

DOUBLE arithmetic_operation(WORD operation, DOUBLE a, DOUBLE b)
{
	switch (operation)
	{
	case IDC_BUTTON_PLUS: a += b; break;
	case IDC_BUTTON_MINUS: a -= b; break;
	case IDC_BUTTON_ASTER: a *= b; break;
	case IDC_BUTTON_SLASH: a /= b; break;
	}
	return a;
}