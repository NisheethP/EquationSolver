#include <Windows.h>
#include "resource.h"
#include <string.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <vector>

typedef long double LDOUBLE;
typedef std::vector<std::vector<LDOUBLE>> LDOUBLEVECTOR;

static TCHAR WindowClass[] = L"WindowProject1";
static TCHAR WindowTitle[] = L"Test 2";
HINSTANCE hInst;

/*
 * FUNCTION PROTOTYPES
 */

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
BOOL GetInputProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
void ErrorExit(LPTSTR lpszFunction);

/*
 * MAIN FUNCTION
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Window Decleration
	//--------------------
	WNDCLASSEX window;

	//Initialising the window
	//------------------------

	window.cbSize = sizeof(WNDCLASSEX);
	window.style = CS_HREDRAW | CS_VREDRAW;
	window.lpfnWndProc = WinProc;
	window.cbClsExtra = 0;
	window.cbWndExtra = 0;
	window.hInstance = hInstance;
	window.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	window.hCursor = LoadCursor(NULL, IDC_ARROW);
	window.hbrBackground = (HBRUSH)4;
	window.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	window.lpszClassName = WindowClass;
	window.hIconSm = LoadIcon(window.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	if (!RegisterClassEx(&window))
	{
		MessageBox(NULL, L"Failed to register window", L"Error", NULL);
		return 1;
	}

	//Creating the window
	//--------------------

	HWND hWnd = CreateWindow
		(WindowClass,
		WindowTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		640,
		512,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	//If window fails to be created
	//------------------------------
	if (!hWnd)
	{
		ErrorExit(L"Call to register class failed");
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);

	//Input Handling
	//-----------------
	MSG message;
	while (GetMessage(&message, NULL, 0, 0) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return (int)message.wParam;
}

/*
 * MAIN WINDOW HANDLING
 */
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_COMMAND:
		switch (wParam)
		{
		case ID_FILE_EXIT:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

}



/*
 * GET LAST ERROR CODE
 */
void ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage
		(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
		);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc
		(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)
		);
	StringCchPrintf
		(
		(LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction,
		dw,
		lpMsgBuf
		);

	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}

