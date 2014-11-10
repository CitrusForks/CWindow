// this is a window derived form CWindow
// just to show that inheritance is possible

#ifndef _DERIVEDWINDOW_H_
#define _DERIVEDWINDOW_H_

#include <Windows.h>
#include "../Views/CWindow.h"

class CDerivedWindow : public CWindow
{
public:
	CDerivedWindow(HINSTANCE hInst, CONST WNDCLASSEX* wcx = NULL) 
		:CWindow(hInst, wcx)
	{ 
		SetWindowTitle("Derived Window");
	};

protected:
	// the message handler for this window
	LRESULT CALLBACK WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_PAINT:
			PAINTSTRUCT ps;
			HDC hdc;

			hdc = BeginPaint(hwnd, &ps); 

			TextOut(hdc, 0, 0, "This is a TEST!!!", 17);

			EndPaint(hwnd, &ps);
			break;
		case WM_DESTROY:
			bWindowClosed = TRUE;
			break;
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}

		return 0;
	};
};

#endif
