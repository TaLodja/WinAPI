#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <Winuser.h>
#include <float.h>
#include <stdio.h>
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

CONST CHAR* IMAGE_DIGIT[]
{
	"Button_1.bmp",
	"Button_2.bmp",
	"Button_3.bmp",
	"Button_4.bmp",
	"Button_5.bmp",
	"Button_6.bmp",
	"Button_7.bmp",
	"Button_8.bmp",
	"Button_9.bmp"
};
CONST CHAR* IMAGE_OPERATION[]
{
	"Button_Plus.bmp",
	"Button_Minus.bmp",
	"Button_Aster.bmp",
	"Button_Slash.bmp",
};

#define BUTTON_SHIFT_X(shift) g_i_BUTTON_START_X+(g_i_BUTTON_SIZE + g_i_INTERVAL)*(shift)
#define BUTTON_SHIFT_Y(shift) g_i_BUTTON_START_Y+(g_i_BUTTON_SIZE + g_i_INTERVAL)*(shift)

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
	//Для отслеживания состояний калькулятора
	static DOUBLE a = DBL_MIN;					//левый операнд; 0 - может быть какое-то промежуточное состояние, это полезная информация
	static DOUBLE b = DBL_MIN;					//правый операнд
	static INT operation = 0;				//нажата ли кнопка операции
	static BOOL input = FALSE;				//флаг, который устанавливается при нажатии на цифру
	static BOOL input_operation = FALSE;	//флаг, который устанавливается при нажатии на кнопку операции
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hEdit = CreateWindowEx
		(
			NULL, "Edit", "0",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
			//WS - Window Style
			//ES - EditControl Style
			10, 10,
			g_i_SCREEN_WIDTH, g_i_SCREEN_HEIGHT,
			hwnd,
			(HMENU)IDC_EDIT,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_SHIFT_X(0), BUTTON_SHIFT_Y(3),
			g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_0,
			GetModuleHandle(NULL),
			NULL
		);
		HBITMAP hButton_0 = (HBITMAP)LoadImage(GetModuleHandle(NULL), "Button_0.bmp", IMAGE_BITMAP, g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE, LR_LOADFROMFILE);
		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_0), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hButton_0);
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
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
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
		HBITMAP hButton_digit[9] = {};
		for (int i = 0; i < 9;i++)
		{
			hButton_digit[i] = (HBITMAP)LoadImage(GetModuleHandle(NULL), IMAGE_DIGIT[i], IMAGE_BITMAP, g_i_BUTTON_SIZE, g_i_BUTTON_SIZE, LR_LOADFROMFILE);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_1 + i), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hButton_digit[i]);
		}
		CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_SHIFT_X(2), BUTTON_SHIFT_Y(3),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_POINT,
			GetModuleHandle(NULL),
			NULL
		);
		HBITMAP hButton_Point = (HBITMAP)LoadImage(GetModuleHandle(NULL), "Button_Point.bmp", IMAGE_BITMAP, g_i_BUTTON_SIZE, g_i_BUTTON_SIZE, LR_LOADFROMFILE);
		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_POINT), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hButton_Point);
		CHAR operation[2] = "";
		for (int i = 0; i < 4; i++)
		{
			operation[0] = g_OPERATIONS[i];
			CreateWindowEx
			(
				NULL, "Button", operation,
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
				BUTTON_SHIFT_X(3), BUTTON_SHIFT_Y(3 - i),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_PLUS + i),
				GetModuleHandle(NULL),
				NULL
			);
		}
		HBITMAP hButton_operation[4] = {};
		for (int i = 0; i < 4;i++)
		{
			hButton_operation[i] = (HBITMAP)LoadImage(GetModuleHandle(NULL), IMAGE_OPERATION[i], IMAGE_BITMAP, g_i_BUTTON_SIZE, g_i_BUTTON_SIZE, LR_LOADFROMFILE);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS + i), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hButton_operation[i]);
		}
		CreateWindowEx
		(
			NULL, "Button", "<-",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_SHIFT_X(4), BUTTON_SHIFT_Y(0),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_BSP,
			GetModuleHandle(NULL),
			NULL
		);
		HBITMAP hButton_BSP = (HBITMAP)LoadImage(GetModuleHandle(NULL), "Button_BSP.bmp", IMAGE_BITMAP, g_i_BUTTON_SIZE, g_i_BUTTON_SIZE, LR_LOADFROMFILE);
		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_BSP), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hButton_BSP);
		CreateWindowEx
		(
			NULL, "Button", "C",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_SHIFT_X(4), BUTTON_SHIFT_Y(1),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_CLR,
			GetModuleHandle(NULL),
			NULL
		);
		HBITMAP hButton_CLR = (HBITMAP)LoadImage(GetModuleHandle(NULL), "Button_CLR.bmp", IMAGE_BITMAP, g_i_BUTTON_SIZE, g_i_BUTTON_SIZE, LR_LOADFROMFILE);
		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_CLR), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hButton_CLR);
		CreateWindowEx
		(
			NULL, "Button", "=",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			BUTTON_SHIFT_X(4), BUTTON_SHIFT_Y(2),
			g_i_BUTTON_SIZE, g_i_BUTTON_DOUBLE_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_EQUAL,
			GetModuleHandle(NULL),
			NULL
		);
		HBITMAP hButton_Equal = (HBITMAP)LoadImage(GetModuleHandle(NULL), "Button_Equal.bmp", IMAGE_BITMAP, g_i_BUTTON_SIZE, g_i_BUTTON_DOUBLE_SIZE, LR_LOADFROMFILE);
		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hButton_Equal);
	}
	break;
	case WM_COMMAND:
	{
		CONST INT SIZE = 256;
		CHAR sz_display[SIZE] = {};
		CHAR sz_digit[2] = {};
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
		SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_display);
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)
		{
			if (input_operation)
				sz_display[0] = 0;
			sz_digit[0] = LOWORD(wParam) - IDC_BUTTON_0 + 48;		//48 - ASCII-код 0
			//SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_display);
			if (strcmp(sz_display, "0"))
				strcat(sz_display, sz_digit);
			else
				strcpy(sz_display, sz_digit);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_display);
			input_operation = FALSE;	//ввод цифр сбрасывает ввод операции
			input = TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_POINT && strchr(sz_display, '.') == NULL)
		{
			strcat(sz_display, ".");
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			//if (a == DBL_MIN) a = atof(sz_display);
			//else b = atof(sz_display);
			if (input)
			{
				(a == DBL_MIN ? a : b) = atof(sz_display);
				SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
			}
			input = FALSE;		//сохранили в память
			operation = LOWORD(wParam);
			input_operation = TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{
			if (a == DBL_MIN) break;
			if (input) b = atof(sz_display);
			switch (operation)
			{
			case IDC_BUTTON_PLUS: a += b; break;
			case IDC_BUTTON_MINUS: a -= b; break;
			case IDC_BUTTON_ASTER: a *= b; break;
			case IDC_BUTTON_SLASH: a /= b; break;
			}
			input = FALSE;
			input_operation = FALSE;
			sprintf(sz_display, "%g", a);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			if (strlen(sz_display) == 1) sz_display[0] = '0';
			else sz_display[strlen(sz_display) - 1] = 0;
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_CLR)
		{
			a = b = DBL_MIN;
			operation = 0;
			input = input_operation = FALSE;
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"0");
		}
	}
	break;
	case WM_KEYDOWN:
	{
		//HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
		if (wParam >= 0x60 && wParam <= 0x69)
			SendMessage(GetDlgItem(hwnd, wParam - 0x60 + IDC_BUTTON_0), BM_SETSTATE, TRUE, 0);
		if (wParam >= 0x30 && wParam <= 0x39)
			SendMessage(GetDlgItem(hwnd, wParam - 0x30 + IDC_BUTTON_0), BM_SETSTATE, TRUE, 0);
		switch (wParam)
		{
		case VK_DECIMAL:
		case VK_OEM_PERIOD:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_POINT), BM_SETSTATE, TRUE, 0);
			break;
		case VK_ADD:
		case VK_OEM_PLUS:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, TRUE, 0);
			break;
		case VK_SUBTRACT:
		case VK_OEM_MINUS:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_MINUS), BM_SETSTATE, TRUE, 0);
			break;
		case VK_MULTIPLY:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, TRUE, 0);
			break;
		case VK_DIVIDE:
		case VK_OEM_2:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_SLASH), BM_SETSTATE, TRUE, 0);
			break;
		case VK_RETURN:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETSTATE, TRUE, 0);
			break;
		case VK_DELETE:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_CLR), BM_SETSTATE, TRUE, 0);
			break;
		case VK_BACK:
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_BSP), BM_SETSTATE, TRUE, 0);
			break;
		case VK_ESCAPE:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		}
	}
	break;
	case WM_KEYUP:
	{
		if (wParam >= 0x60 && wParam <= 0x69)
		{
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - 0x60 + IDC_BUTTON_0), 0);
			SendMessage(GetDlgItem(hwnd, wParam - 0x60 + IDC_BUTTON_0), BM_SETSTATE, FALSE, 0);
		}
		if (wParam >= 0x30 && wParam <= 0x39)
		{
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - 0x30 + IDC_BUTTON_0), 0);
			SendMessage(GetDlgItem(hwnd, wParam - 0x30 + IDC_BUTTON_0), BM_SETSTATE, FALSE, 0);
		}
		switch (wParam)
		{
		case VK_DECIMAL:
		case VK_OEM_PERIOD:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_POINT), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_POINT), BM_SETSTATE, FALSE, 0);
			break;
		case VK_ADD:
		case VK_OEM_PLUS:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_PLUS), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, FALSE, 0);
			break;
		case VK_SUBTRACT:
		case VK_OEM_MINUS:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_MINUS), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_MINUS), BM_SETSTATE, FALSE, 0);
			break;
		case VK_MULTIPLY:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_ASTER), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, FALSE, 0);
			break;
		case VK_DIVIDE:
		case VK_OEM_2:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_SLASH), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_SLASH), BM_SETSTATE, FALSE, 0);
			break;
		case VK_RETURN:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETSTATE, FALSE, 0);
			break;
		case VK_DELETE:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_CLR), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_CLR), BM_SETSTATE, FALSE, 0);
			break;
		case VK_BACK:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_BSP), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_BSP), BM_SETSTATE, FALSE, 0);
			break;
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