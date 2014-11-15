#include "CWndClass.h"
#include "CExample.h"



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// create the Window Class
	CWndClass *wndClass = new CWndClass(hInstance, TEXT("My window class"));
	wndClass->Register();

	// create the window, 
	CExample *wnd = new CExample(TEXT("Generic Window Title"), wndClass->className());
	wnd->Create();

	// show the window on the screen
	wnd->Show();

	// pump messages:
	MSG  msg;
	int status;
	while ((status = ::GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if (status == -1) {
			// handle the error, break
			break;
		}
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return msg.wParam;
}



