#include "CWindow.h"
#include <tchar.h>

CWindow::CWindow() {
	this->_hwnd = NULL;
	this->_lpCaption = NULL;
	this->_lpClassName = NULL;
}

CWindow::CWindow(LPCSTR lpszCaption, LPWNDCLASSEX lpwndClassEx) {

	this->SetCaption(lpszCaption);
	this->SetClassName(lpwndClassEx->lpszClassName);
	this->_hInstance = lpwndClassEx->hInstance;

	this->Register(lpwndClassEx);
}

CWindow::CWindow(LPCSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName) {
	WNDCLASSEX wnd;

	this->SetCaption(lpszCaption);
	this->SetClassName(lpszClassName);
	this->_hInstance = hInstance;

	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wnd.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
	wnd.lpszMenuName = NULL;

	this->Register(&wnd);
}

CWindow::CWindow(LPCSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
	LPCTSTR lpszMenuName, HICON hIconSm) {
	
	WNDCLASSEX wnd;

	this->SetCaption(lpszCaption);
	this->SetClassName(lpszClassName);
	this->_hInstance = hInstance;

	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.style = style;
	wnd.hbrBackground = hbrBackground;
	wnd.hCursor = hCursor;
	wnd.hIcon = hIcon;
	wnd.hIconSm = hIconSm;
	wnd.lpszMenuName = lpszMenuName;

	this->Register(&wnd);
}

CWindow::~CWindow() {
	if (this->_hwnd != NULL) {
		DestroyWindow(this->_hwnd);
		this->_hwnd = NULL;
	}

	if (this->_lpClassName != NULL){
		free((LPVOID)this->_lpClassName);
		this->_lpClassName = NULL;
	}

	if (this->_lpCaption != NULL){
		free((LPVOID)this->_lpCaption);
		this->_lpCaption = NULL;
	}
}

VOID CWindow::Register(LPWNDCLASSEX lpwndClassEx) {

	lpwndClassEx->lpfnWndProc = CWindow::msgRouter;
	lpwndClassEx->lpszClassName = this->_lpClassName;
	lpwndClassEx->hInstance = this->_hInstance;

	if (!RegisterClassEx(lpwndClassEx)) {
		throw "Could not register the window class.";
	}
}

VOID CWindow::SetClassName(LPCTSTR lpszClassName) {
	int nLen;

	if (lpszClassName != NULL && this->_hwnd == NULL) {
		if (this->_lpClassName != NULL) {
			free((LPVOID)this->_lpClassName);
		}

		nLen = _tcsnlen(lpszClassName, 255) + 1;
		this->_lpClassName = (LPTSTR)calloc(nLen, sizeof(TCHAR));
		_tcscpy_s(this->_lpClassName, nLen, lpszClassName);
	}
	else {
		throw "Invalid class name";
	}
}

LPCTSTR CWindow::GetCaption() {
	return this->_lpCaption == NULL ? TEXT("") : this->_lpCaption;
}

VOID CWindow::SetCaption(LPCTSTR lpszCaption) {
	int nLen;

	if (lpszCaption != NULL && this->_hwnd == NULL) {
		if (this->_lpCaption != NULL) {
			free((LPVOID)this->_lpCaption);
		}

		nLen = _tcsnlen(lpszCaption, 255) + 1;
		this->_lpCaption = (LPTSTR)calloc(nLen , sizeof(TCHAR));
		_tcscpy_s(this->_lpCaption, nLen, lpszCaption);
	}
	else {
		throw "Invalid caption text";
	}
}

LPCTSTR CWindow::GetClassName() {
	return this->_lpClassName == NULL ? TEXT("") : this->_lpClassName;
}

HWND CWindow::Create() {
	return this->Create(
			(HWND)NULL,
			0L,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			(HMENU)NULL,
			NULL
		);
}

HWND CWindow::Create(HWND hWndParent) {

	return this->Create(
			(HWND)hWndParent,
			0L,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			(HMENU)NULL,
			NULL
		);
}

HWND CWindow::Create(HWND hWndParent, DWORD dwExStyle, DWORD dwStyle,
	INT x, INT y, INT nWidth, INT nHeight, HMENU hMenu, LPVOID lpParam) {
	
	if (this->_hwnd == NULL) {
		this->_hwnd = CreateWindowEx(
			dwExStyle,
			this->_lpClassName,
			this->_lpCaption,
			dwStyle,
			x,
			y,
			nWidth,
			nHeight,
			(HWND)hWndParent,
			(HMENU)hMenu,
			this->_hInstance,
			this
			);
		
		if (this->_hwnd == NULL) {
			throw "The window could not be created";
		}

		this->_lParam = lpParam;
	}

	return this->_hwnd;
}

BOOLEAN CWindow::Show(int nCmdShow) {
	return (this->_hwnd == NULL) ? FALSE : ShowWindow(this->_hwnd, nCmdShow);
}

BOOLEAN CWindow::Show() {
	return this->Show(SW_SHOW);
}

BOOLEAN CWindow::Hide() {
	return this->Show(SW_HIDE);
}

BOOLEAN CWindow::Maximize() {
	return this->Show(SW_MAXIMIZE | SW_SHOW);
}

BOOLEAN CWindow::Minimize(){
	return this->Show(SW_MINIMIZE | SW_SHOW);
}

BOOLEAN CWindow::Restore(){
	return this->Show(SW_RESTORE | SW_SHOW);
}



LRESULT CALLBACK CWindow::msgRouter(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	CWindow *wnd = 0;

	if (uMsg == WM_NCCREATE) {
		::SetWindowLong(hwnd, GWL_USERDATA, long((LPCREATESTRUCT(lParam))->lpCreateParams));
	}

	wnd = (CWindow *)(::GetWindowLong(hwnd, GWL_USERDATA));

	if (wnd)
		return wnd->wndProc(hwnd, uMsg, wParam, lParam);
	else
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}