#if !defined __CEXAMPLE_H__
#define __CEXAMPLE_H__

#include "CWindow.h"
#include "CWndClass.h"

class CExample : public CWindow {
public:
	CExample(const TCHAR *windowName, const TCHAR *className);

	virtual LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

	void Show();
};


#endif