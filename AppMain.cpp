
#include "CMainWindow.h"



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	CMainWindow *lpcwndExample = new CMainWindow(TEXT("ShellNew"), hInstance, TEXT("ShellNewMainWindow"));

	lpcwndExample->Create();

	lpcwndExample->Show();

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



