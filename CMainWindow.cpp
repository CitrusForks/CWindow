#include "CMainWindow.h"
#include <Commctrl.h>

CMainWindow::CMainWindow(LPCSTR lpszCaption, LPWNDCLASSEX lpwndClassEx) 
: CWindow( lpszCaption, lpwndClassEx)
{

}

CMainWindow::CMainWindow(LPCSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName) 
:CWindow(lpszCaption, hInstance, lpszClassName) {

}

CMainWindow::CMainWindow(LPCSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
	LPCTSTR lpszMenuName, HICON hIconSm)
	: CWindow(lpszCaption, hInstance, lpszClassName,
	style, hIcon, hCursor, hbrBackground,
	lpszMenuName, hIconSm)
{

}


LRESULT CALLBACK CMainWindow::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static HWND hwndStatics[] = { NULL, NULL };
	static HWND hwndStatus = NULL;

	RECT rc;
	int statusHeight;
	long style;

	switch (uMsg)
	{
	case WM_CREATE:
		GetWindowRect(NULL, &rc);
		SetWindowPos(
			hwnd, 
			0, 
			GetSystemMetrics(SM_CXSCREEN) / 2 - 320, 
			GetSystemMetrics(SM_CYSCREEN) / 2 - 240, 
			640, 
			480, 
			SWP_NOZORDER
		);

		
		style = GetWindowLong(hwnd, GWL_STYLE);
		style &= ~WS_MAXIMIZEBOX;
		style &= ~WS_MINIMIZEBOX;
		SetWindowLong(hwnd, GWL_STYLE, style);

		InitCommonControls();

		hwndStatics[0] = CreateWindowEx(0L, "Static", "Lista", WS_CHILD | WS_VISIBLE | SS_ETCHEDFRAME, 10, 10, 100, 100, hwnd, (HMENU)NULL, this->_hInstance, (LPVOID)NULL);
		hwndStatics[1] = CreateWindowEx(0L, "Static", "Static", WS_CHILD | WS_VISIBLE | SS_ETCHEDFRAME, 110, 10, 100, 100, hwnd, (HMENU)NULL, this->_hInstance, (LPVOID)NULL);

		hwndStatus = CreateWindowEx(0,STATUSCLASSNAME,(PCTSTR)NULL,SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, (HMENU)0L, this->_hInstance,NULL);                  

		return 0;
	case WM_SIZE:
		GetWindowRect(hwndStatus, &rc);
		statusHeight = rc.bottom - rc.top;
		SendMessage(hwndStatus, WM_SIZE, 0, 0);

		GetClientRect(hwnd, &rc);
		
		rc.right -= rc.left;
		rc.bottom -= rc.top;

		SetWindowPos(hwndStatics[0], 0, rc.left + 10, rc.top + 10, rc.right / 2 - 15, rc.bottom - 20 - statusHeight, SWP_NOZORDER);
		SetWindowPos(hwndStatics[1], 0, rc.right / 2 + 5, rc.top + 10, rc.right / 2 - 15, rc.bottom - 20 - statusHeight, SWP_NOZORDER);

		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	case WM_GETMINMAXINFO:
		((LPMINMAXINFO)lParam)->ptMaxSize.x = 640;
		((LPMINMAXINFO)lParam)->ptMaxSize.y = 480;
		((LPMINMAXINFO)lParam)->ptMaxTrackSize.x = 640;
		((LPMINMAXINFO)lParam)->ptMaxTrackSize.y = 480;
		((LPMINMAXINFO)lParam)->ptMinTrackSize.x = 320;
		((LPMINMAXINFO)lParam)->ptMinTrackSize.y = 240;
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	default:
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
