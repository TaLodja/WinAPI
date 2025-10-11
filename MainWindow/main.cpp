#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <cstdio>		//Функции написаны на языке С++
//#include <stdio.h>	//Функции написаны на языке С
#include "resource.h"

CONST CHAR g_sz_WND_CLASS_NAME[] = "My Windows Class";
//CONST INT XMonitor = GetSystemMetrics(SM_CXSCREEN);
//CONST INT YMonitor = GetSystemMetrics(SM_CYSCREEN);

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbWndExtra = 0;
	wClass.cbClsExtra = 0;

	//wClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//wClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hIconSm = (HICON)LoadImage(hInstance, "bitcoin.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wClass.hIcon = (HICON)LoadImage(hInstance, "2palms.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wClass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wClass.hCursor = (HCURSOR)LoadImage(hInstance, "Working In Background.ani", IMAGE_CURSOR, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wClass.hInstance = hInstance;
	wClass.lpszClassName = g_sz_WND_CLASS_NAME;
	wClass.lpszMenuName = NULL;
	wClass.lpfnWndProc = WndProc;

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	//INT WidthWindow = XMonitor*0.75;
	//INT HeightWindow = YMonitor *0.75;
	//INT StartX = (XMonitor - WidthWindow) / 2;
	//INT StartY = (YMonitor - HeightWindow) / 2;
	//CHAR gz_message[FILENAME_MAX] = {};
	//sprintf(gz_message, "%s. Размер окна: %i x %i. Положение окна: X = %i, Y = %i",
	//	g_sz_WND_CLASS_NAME, WidthWindow, HeightWindow, StartX, StartY);

	//Решение задания в классе:
	INT screen_width = GetSystemMetrics(SM_CXSCREEN);
	INT screen_height = GetSystemMetrics(SM_CYSCREEN);

	INT window_width = screen_width / 4 * 3;
	INT window_height = screen_height * .75;

	INT window_start_x = screen_width * (0.25 / 2);
	INT window_start_y = screen_height * 0.125;

	HWND hwnd = CreateWindowEx		//Дескриптор окна
	(
		NULL,						//exStyles
		g_sz_WND_CLASS_NAME,		//Class name
		g_sz_WND_CLASS_NAME,
		//gz_message,					//Window title (Заголовок окна)
		WS_OVERLAPPEDWINDOW,		//Стиль окна
		window_start_x, window_start_y,
		//StartX, StartY,				//Window position
		window_width, window_height,
		//WidthWindow, HeightWindow,	//Window size
		NULL,						//Родительское окно
		NULL,
		hInstance,
		NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);	//Задает режим отображения окна
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
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
		HWND hStatic = CreateWindowEx
		(
			NULL,
			"Static",
			"Этот Static-text создан при помощи функции CreateWindowEx",
			WS_CHILD | WS_VISIBLE,
			10, 10,
			500, 25,
			hwnd,
			(HMENU)1000,
			GetModuleHandle(NULL),
			NULL
		);
		//WS - WindowStyle
		//ES - EditControlStyle
		//WS_CHILD - показывает, что создаваемое окно является дочерним элементом интерфейса какого-то другого окна
		HWND hEdit = CreateWindowEx
		(
			NULL,
			"Edit",
			"",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			10, 45,
			500, 22,
			hwnd,
			(HMENU)1001,
			GetModuleHandle(NULL),
			NULL
		);
		HWND hButton = CreateWindowEx
		(
			NULL,
			"Button",
			"Применить",
			WS_CHILD | WS_VISIBLE,
			430, 70,
			80, 32,
			hwnd,
			(HMENU)1002,
			GetModuleHandle(NULL),
			NULL
		);

		//CHAR gz_message_size[FILENAME_MAX] = {};
		//sprintf(gz_message_size, "Размеры монитора: %i x %i", XMonitor, YMonitor);

		HWND hStaticSizeWindow = CreateWindowEx
		(
			NULL,
			"Static",
			"",
			//gz_message_size,
			WS_CHILD | WS_VISIBLE,
			10, 90,
			500, 22,
			hwnd,
			(HMENU)1003,
			GetModuleHandle(NULL),
			NULL
		);
	}
	break;
	case WM_SIZE:
	case WM_MOVE:
	{
		RECT wnd_rect;		//Структура, описыыающая прямоугольник окна
		GetWindowRect(hwnd, &wnd_rect);	//lp - long pointer
		CHAR sz_title[256] = {};
		sprintf
		(
			sz_title, "%s Size: %ix%i. Position: %ix%i", 
			g_sz_WND_CLASS_NAME, 
			wnd_rect.right - wnd_rect.left, wnd_rect.bottom-wnd_rect.top, 
			wnd_rect.left, wnd_rect.top
		);
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_title);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1002:
		{
			MessageBox(hwnd, "Привет", "Привет", MB_OK | MB_ICONINFORMATION);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hStatic = GetDlgItem(hwnd, 1000);
			HWND hEdit = GetDlgItem(hwnd, 1001);

			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hStatic, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			//SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			SendMessage(GetDlgItem(hwnd, 1002), WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		}
		break;
	case WM_DESTROY:
		MessageBox(NULL, "Лучше двери закройте...", "Finita la comedia", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		//DestroyWindow(hwnd);
		if (MessageBox(hwnd, "Вы действительно хотите закрыть окно?", "Че, внатуре?", MB_YESNO | MB_ICONQUESTION) == IDYES)
			SendMessage(hwnd, WM_DESTROY, 0, 0);
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}