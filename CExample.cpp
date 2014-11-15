#include "CExample.h"

CExample::CExample(const TCHAR *windowName, const TCHAR *className) {
	_windowName = windowName;
	_className = className;
	_hInstance = ::GetModuleHandle(NULL);
	_style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	_y = _x = CW_USEDEFAULT;
	_height = _width = CW_USEDEFAULT;
	_styleEx = 0;
	_hwndParent = 0;
	_hMenu = 0;
}

LRESULT CALLBACK CExample::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HINSTANCE _hInstance;

	switch (uMsg)
	{
	case WM_CREATE:
		_hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	default:
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void CExample::Show() {
	::ShowWindow(_hwnd, SW_SHOW);
	::UpdateWindow(_hwnd);
}
