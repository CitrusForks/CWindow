#include "CMainWindow.h"

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
	CMainWindow * pHwnd;

	switch (uMsg)
	{
	case WM_CREATE:
		
		pHwnd = new CMainWindow(TEXT("Hija"), this->_hInstance, TEXT("vhija"));

		pHwnd->Create(hwnd);

		SetClassLong(pHwnd->_hwnd, GCLP_HBRBACKGROUND, (LONG)GetStockObject(BLACK_BRUSH));

		pHwnd->Show();

		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	default:
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
