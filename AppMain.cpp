#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "Test/DerivedWindow.h"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// create an instance of the derived window class (inherited from CWindow)
	CDerivedWindow mainWnd(hInstance);

	// make this derived window a little different than CWindow
    WNDCLASSEX wcx; 
 
    // Fill in the window class structure with default parameters 
    wcx.cbSize = sizeof(WNDCLASSEX);							// size of structure 
    wcx.style = CS_HREDRAW | CS_VREDRAW;						// redraw if size changes 
    wcx.lpfnWndProc = CWindow::stWinMsgHandler;				// points to window procedure 
    wcx.cbClsExtra = 0;											// no extra class memory 
    wcx.cbWndExtra = 0;											// no extra window memory 
    wcx.hInstance = hInstance;									// handle to instance 
    wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// predefined app. icon 
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);					// predefined arrow 
    wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// white background brush 
    wcx.lpszMenuName = NULL;									// name of menu resource 
    wcx.lpszClassName = "DerivedWindow";						// name of window class 
    wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);				// small class icon 

	// register the window
	if (mainWnd.RegisterWindow(&wcx))
	{
		DWORD dwError = 0;
		DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		RECT rc;

		rc.top = 100;
		rc.left = 100;
		rc.right = 740;
		rc.bottom = 580;

		// create the window and start the message loop
		// we will get kicked out of the message loop when the window closes
		if (mainWnd.Create(dwStyle, &rc))
		{
			// message loop
			MSG msg;
			BOOL bRet;

			while((bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
			{ 
				if (bRet == -1)
				{
					// handle the error and possibly exit
					break;
				}
				else
				{
					TranslateMessage(&msg); 
					DispatchMessage(&msg); 

					if (mainWnd.IsWindowClosed()) 
						break;
				}
			}
		}
		else
			dwError = GetLastError();
	}

	return 0;
}
