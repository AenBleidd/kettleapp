// Kettle.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Kettle.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND hBWnd1, hBWnd2, hEWnd, hSWnd1, hSWnd2;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_KETTLE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KETTLE));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KETTLE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_KETTLE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   HDC dc;
   hInst = hInstance; // Store instance handle in our global variable
   int x, y, cx, cy;
   cx = 266;
   cy = 120;
   x = GetSystemMetrics(SM_CXSCREEN) / 2 - cx / 2;
   y = GetSystemMetrics(SM_CYSCREEN) / 2 - cy / 2;

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
      x, y, cx, cy, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   dc = GetDC(hWnd);
   hBWnd1 = CreateWindowA("BUTTON", "Поставить таймер", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 20, 260, 50, hWnd, (HMENU)IDB_GO_BUTTON, hInstance, NULL);
   hEWnd = CreateWindowA("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_NUMBER, 140, 0, 79, 20, hWnd, (HMENU)IDE_EDIT, hInstance, NULL);
   hSWnd1 = CreateWindowA("static", "Поставить таймер на", WS_VISIBLE | WS_CHILD, 0, 0, 140, 20, hWnd, NULL, hInstance, NULL);
   hSWnd2 = CreateWindowA("static", "минут", WS_VISIBLE | WS_CHILD, 220, 0, 40, 20, hWnd, NULL, hInstance, NULL); 
   SendMessageA(hEWnd, EM_LIMITTEXT, 4, 0);
   SetFocus(hEWnd);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, nID, nTime;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDB_GO_BUTTON:
			char chBuff[3];
            DWORD cbText;
            * (WORD *) chBuff = sizeof (chBuff) - 1;
            cbText = SendMessageA(hEWnd, EM_GETLINE, 0, (LPARAM)chBuff);
            chBuff[cbText] = '\0';
			if(cbText == 0)
			{
				MessageBoxA(NULL, "Вы должны ввести количество времени, на которое хотите поставить таймер.", "Внимание!", MB_OK | MB_ICONEXCLAMATION);
				break;
			}
			nTime = atoi(chBuff);
			nTime *= 60000;
			DestroyWindow(hSWnd1);
			DestroyWindow(hSWnd2);
			DestroyWindow(hEWnd);
			DestroyWindow(hBWnd1);
			DestroyWindow(hBWnd2);
			hBWnd2 = CreateWindowA("BUTTON", "Остановить таймер", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 0, 260, 70, hWnd, (HMENU)IDB_EXIT_BUTTON, hInst, NULL); 
			nID = SetTimer(hWnd, IDT_TIMER, nTime, NULL);
			break;
		case IDB_EXIT_BUTTON:
			KillTimer(hWnd, IDT_TIMER);
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_TIMER:
		ShowWindow(hWnd, SW_SHOWNORMAL);
		KillTimer(hWnd, IDT_TIMER);
		FILE *file;
		file=fopen("sound.wav","r");
		if(file != 0) sndPlaySoundA("sound.wav", SND_LOOP|SND_ASYNC), fclose(file);
		else PlayWAVResource(NULL, ID_SOUND, "sound", FALSE);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hPWnd;
	HINSTANCE hDLL;
	HBITMAP hb;
	char text[100];
	hDLL = LoadLibraryA("functions.dll");
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		hb = LoadBitmapA(hDLL, MAKEINTRESOURCEA(IDB_PHOTO)); 
		hPWnd = CreateWindowA("static", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_BITMAP, 0, 80, 0, 0, hDlg, NULL, hInst, NULL);
		CreateWindowA("static", "Программа 'Чайник' v 1.2 build 05.04.2009", WS_VISIBLE | WS_CHILD | SS_CENTER, 105, 0, 285, 20, hDlg, NULL, hInst, NULL);
		CreateWindowA("static", "Разработчик Lestat de Lioncourt", WS_VISIBLE | WS_CHILD | SS_CENTER, 105, 20, 285, 20, hDlg, NULL, hInst, NULL);
		CreateWindowA("static", "Если Вам понравилась данная программа", WS_VISIBLE | WS_CHILD | SS_CENTER, 105, 40, 285, 20, hDlg, NULL, hInst, NULL);
		CreateWindowA("static", "и Вы хотите помочь разработчику -", WS_VISIBLE | WS_CHILD | SS_CENTER, 105, 60, 285, 20, hDlg, NULL, hInst, NULL);
		CreateWindowA("static", "перешлите любую сумму на один из", WS_VISIBLE | WS_CHILD | SS_CENTER, 105, 80, 285, 20, hDlg, NULL, hInst, NULL);
		CreateWindowA("static", "следующих кошельков:", WS_VISIBLE | WS_CHILD | SS_CENTER, 105, 100, 285, 20, hDlg, NULL, hInst, NULL);
		LoadStringA(hDLL, IDS_WMZ, text, 100);  
		CreateWindowA("static", text, WS_VISIBLE | WS_CHILD | SS_CENTER, 105, 120, 285, 20, hDlg, NULL, hInst, NULL);
		LoadStringA(hDLL, IDS_WMR, text, 100);  
		CreateWindowA("static", text, WS_VISIBLE | WS_CHILD | SS_CENTER, 105, 140, 285, 20, hDlg, NULL, hInst, NULL);
		LoadStringA(hDLL, IDS_WMU, text, 100);  
		CreateWindowA("static", text, WS_VISIBLE | WS_CHILD | SS_CENTER, 105, 160, 285, 20, hDlg, NULL, hInst, NULL);
		LoadStringA(hDLL, IDS_WME, text, 100);  
		CreateWindowA("static", text, WS_VISIBLE | WS_CHILD | SS_CENTER, 105, 180, 285, 20, hDlg, NULL, hInst, NULL);
		LoadStringA(hDLL, IDS_WMG, text, 100);  
		CreateWindowA("static", text, WS_VISIBLE | WS_CHILD | SS_CENTER, 105, 200, 285, 20, hDlg, NULL, hInst, NULL);
		CreateWindowA("static", "СПАСИБО!", WS_VISIBLE | WS_CHILD | SS_CENTER, 105, 220, 285, 20, hDlg, NULL, hInst, NULL);
		CreateWindowA("BUTTON", "Посетить сайт разработчика", WS_VISIBLE | WS_CHILD, 105, 240, 285, 30, hDlg, (HMENU)IDSITE, hInst, NULL);
		CreateWindowA("BUTTON", "Закрыть", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 105, 270, 285, 30, hDlg, (HMENU)IDOK, hInst, NULL);
		SendMessageA(hPWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hb);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if(LOWORD(wParam) == IDSITE)
		{
			LoadStringA(hDLL, IDS_URL, text, 100);
			ShellExecuteA(NULL, "open", text, NULL, NULL, SW_SHOWNORMAL);
		}
		break;
	}
	return (INT_PTR)FALSE;
}
