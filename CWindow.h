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
	HINSTANCE _hInstance;
	LPTSTR _lpClassName;
	LPTSTR _lpCaption;
	LPVOID _lParam;
	WNDPROC _wndprocDefault;

	CWindow();
	virtual VOID SetClassName(LPCTSTR lpszClassName);
	virtual VOID Register(LPWNDCLASSEX lpwndClassEx);
public:
	CWindow(LPCSTR lpszCaption, LPWNDCLASSEX lpwndClassEx);
	CWindow(LPCSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName);
	CWindow(LPCSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
		UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground, 
		LPCTSTR lpszMenuName, HICON hIconSm);

	~CWindow();

	virtual LPCTSTR GetCaption();
	virtual VOID SetCaption(LPCTSTR lpszCaption);
	virtual LPCTSTR GetClassName();

	virtual HWND Create();
	virtual HWND Create(HWND hWndParent);
	virtual HWND Create(HWND hWndParent, DWORD dwExStyle, DWORD dwStyle,
		INT x, INT y, INT nWidth, INT nHeight, HMENU hMenu, LPVOID lpParam);

	virtual BOOLEAN Show(int nCmdShow);
	virtual BOOLEAN Show();
	virtual BOOLEAN Hide();
	virtual BOOLEAN Maximize();
	virtual BOOLEAN Minimize();
	virtual BOOLEAN Restore();

	static LRESULT CALLBACK msgRouter(HWND, UINT, WPARAM, LPARAM);

	virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM) = 0;
};

#endif