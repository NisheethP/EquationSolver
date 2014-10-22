#include <Windows.h>
#include "resource.h"
#include <string.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <vector>
#include "Matrix.h"
#include <string>

const UINT SHOWOUTPUT = 20000;

using std::vector;

typedef std::vector<std::vector<double>> VECTOR2D;
VECTOR2D coeffs(3, std::vector<double>(4, 0));

static TCHAR WindowClass[] = L"WindowProject1";
static TCHAR WindowTitle[] = L"Equation Solver";
HINSTANCE hInst;
HWND g_hGetInput = NULL;
HWND g_hShowOutput = NULL;

/*
 * FUNCTION PROTOTYPES
 */
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK GetInputProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK OutputProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

void ErrorExit(LPTSTR lpszFunction);
int Conv_2Dto1D(int row, int col, int numCol = 4);
void TruncateNum(std::wstring& str);

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
	window.hbrBackground = (HBRUSH)(CTLCOLOR_DLG + 1);
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
		g_hGetInput = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_GETINPUT), hWnd, GetInputProc);
		
		if (g_hGetInput != NULL)
		{
			ShowWindow(g_hGetInput, SW_SHOW);
		}

		else
		{
			MessageBox(hWnd, L"CreateDialog returned NULL", L"Warning!",
				MB_OK | MB_ICONINFORMATION);
		}

		g_hShowOutput = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OUTPUT), hWnd, OutputProc);
		if (g_hGetInput != NULL)
		{
			ShowWindow(g_hGetInput, SW_SHOW);
		}

		else
		{
			MessageBox(hWnd, L"CreateDialog returned NULL", L"Warning!",
				MB_OK | MB_ICONINFORMATION);
		}
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
 * HANDLING INPUT WINDOW FOR GETTING COEFFICIENTS
 */
BOOL CALLBACK GetInputProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_INITDIALOG:
		for (int rowIter = 0; rowIter < 3; rowIter++)
		{
			for (int colIter = 0; colIter < 4; colIter++)
			{
				SetDlgItemInt(hwnd, IDC_EDIT1 + Conv_2Dto1D(rowIter, colIter), 0, TRUE);
			}
		}
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case IDC_SOLVE:
			for (int rowIter = 0; rowIter < 3; rowIter++)
			{
				for (int colIter = 0; colIter < 4; colIter++)
				{
					int x =	GetDlgItemInt(hwnd, IDC_EDIT1 + Conv_2Dto1D(rowIter, colIter), NULL, TRUE);
					coeffs[rowIter][colIter] = x;
				}
			}
			SendNotifyMessage(g_hShowOutput, SHOWOUTPUT, NULL, NULL);
			ShowWindow(g_hGetInput, SW_HIDE);
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}


/*
 * SHOWS THE OUTPUT
 */
BOOL CALLBACK OutputProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_INITDIALOG:
		break;
	case SHOWOUTPUT:
	{
		for (int rowIter = 0; rowIter < 3; rowIter++)
		{
			for (int colIter = 0; colIter < 4; colIter++)
			{
				SetDlgItemInt(hwnd, IDC_COEFF1 + Conv_2Dto1D(rowIter, colIter), coeffs[rowIter][colIter], TRUE);
			}
		}
		Solution<3> soln = SolveEqn<3>(Matrix(coeffs));
		for (int i = 0; i < 3; i++)
		{
			std::wstring str = std::to_wstring(soln[i]);
			TruncateNum(str);
			SetDlgItemText(g_hShowOutput, IDC_VAULE1 + i, (LPCWSTR)str.c_str());
		}
		ShowWindow(g_hShowOutput, SW_SHOW);
		break;
	}
	default:
		return FALSE;
	}
	return TRUE;
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

/*
 * CONVERT 2D ARRAY LOC TO 1D ARRAY LOC
 */
int Conv_2Dto1D(int row, int col, int numCol /* = 4*/)
{
	return (numCol*row + col);
}

/*
 *TRUNCATE THE STRING OF A NUMBER TO REMOVE TRAILING ZEROES
 */
void TruncateNum(std::wstring& str)
{
	int Size = str.size();
	for (int i = Size - 1; i >= 0; i--)
	{
		if (str.at(i) == '.')
		{
			str.pop_back();
			break;
		}
		else if (str.at(i) != '0')
			break;
		else if (str.at(i) == '0')
			str.pop_back();
	}
}