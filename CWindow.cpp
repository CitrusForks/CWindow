#include "CWindow.h"

CWindow::CWindow() {

}

LRESULT CALLBACK CWindow::msgRouter(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	CWindow *wnd = 0;

	if (uMsg == WM_NCCREATE) {
		SetWindowLong(hwnd, GWL_USERDATA, long((LPCREATESTRUCT(lParam))->lpCreateParams));
	}

	wnd = (CWindow*)(GetWindowLong(hwnd, GWL_USERDATA));

	if (wnd)
		return wnd->wndProc(hwnd, uMsg, wParam, lParam);
	else
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool CWindow::Create() {
	_hwnd = ::CreateWindowEx(
				_styleEx,
				_className,
				_windowName,
				_style,
				_x,
				_y,
				_width,
				_height,
				_hwndParent,
				_hMenu,
				_hInstance,
				this
			);

	if (!_hwnd) 
		return false;
	else 
		return true;
}
