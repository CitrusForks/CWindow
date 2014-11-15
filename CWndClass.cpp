#include "CWndClass.h"


CWndClass::CWndClass(HINSTANCE hInst, const TCHAR *className)
{
	hInstance = hInst;
	lpfnWndProc = CWindow::msgRouter;
	lpszClassName = className;

	lpszMenuName = 0;
	cbSize = sizeof (WNDCLASSEX);
	cbClsExtra = 0;
	cbWndExtra = 0;
	style = 0;
	hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	hIconSm = ::LoadIcon(NULL, IDI_APPLICATION);
	hCursor = ::LoadCursor(NULL, IDC_ARROW);
	hbrBackground = (HBRUSH) ::GetStockObject(COLOR_BTNFACE);
}

bool CWndClass::Register()
{
	if (::RegisterClassEx(this))
		return true;
	else
		return false;
}
