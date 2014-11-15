#if !defined __CWINDOW_H__
#define __CWINDOW_H__

#include <Windows.h>

/// <summary>
/// Base class for creating windows easily and quickly.</summary>
/// <remarks>
/// It provides a static method that it forwards the window messages towards a 
/// pure virtual window procedure which must be implemented in the derived 
/// classes to process the window messages.</remarks>
class CWindow {
protected:
	HWND _hwnd;
	DWORD _styleEx;
	LPCTSTR _className;
	LPCTSTR _windowName;
	DWORD _style;
	int _x;
	int _y;
	int _width;
	int _height;
	HWND _hwndParent;
	HMENU _hMenu;
	HINSTANCE _hInstance;

public:
	CWindow();

	static LRESULT CALLBACK msgRouter(HWND, UINT, WPARAM, LPARAM);

	virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM) = 0;

	virtual bool Create();
};

#endif