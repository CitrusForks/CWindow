#if !defined __CMainWindow_H__
#define __CMainWindow_H__

#include "CWindow.h"
#include "CWndClass.h"

class CMainWindow : public CWindow {


public:
	CMainWindow(LPCSTR lpszCaption, LPWNDCLASSEX lpwndClassEx);
	CMainWindow(LPCSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName);
	CMainWindow(LPCSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
		LPCTSTR lpszMenuName, HICON hIconSm);

	virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
};


#endif