#include <Windows.h>
#include "resource.h"

CONST CHAR g_sz_WND_CLASS_NAME[] = "My Windows Class";

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

	HWND hwnd = CreateWindowEx	//Дескриптор окна
	(
		NULL,					//exStyles
		g_sz_WND_CLASS_NAME,	//Class name
		g_sz_WND_CLASS_NAME,	//Window title (Заголовок окна)
		WS_OVERLAPPEDWINDOW,	//Стиль окна
		CW_USEDEFAULT, CW_USEDEFAULT,	//Window position
		CW_USEDEFAULT, CW_USEDEFAULT,	//Window size
		NULL,					//Родительское окно
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
			SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_buffer);
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