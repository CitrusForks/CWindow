#if !defined __CWNDCLASS_H__
#define __CWNDCLASS_H__

#include "CWindow.h"

class CWndClass : protected WNDCLASSEX {
public:
	CWndClass(HINSTANCE hInst, const TCHAR * className);

	virtual bool Register();
	virtual const TCHAR * className() const { return lpszClassName; }
};

#endif