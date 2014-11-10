# **CWindow** Proyect

The objetive of this project is write a Win32 window wrapper class which requires the minimun user interventionm, and yet, it be extremely fexible. Also, user must be able to extend the class without having to inherit or reimplement any methods.

Otherwise, wherever possible and reasonable, proyect also tried to use names consistent with the Win32 API as a form of assitance to the developer.   

## Motivation

Anyone familiar with the Win32 API knows that the process for creating windows is rather lengthy: you create a WNDCLASSEX structure, initialize its members and register it (per window class); create the window, check for the validity of the returned handle, and finally display the window. Of course, I neglected to mention that initializing the WNDCLASSEX structures members requires that you define a window procedure first... 

So it's logical that the enterprising C++ developer would like to wrap all of the above in a nice class that would allow her write something like:

```
**CWindow** *wnd = new Window(...); 
wnd->Create(...); 
wnd->Show(...);
```

## Code example

Using this project as as base for applications, the code of these might look like this:

```
#include "CWindow.h"

int APIENTRY WinMain (HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, INT nShow) {
	MSG Msg;
	CWindow mainWnd;

	mainWnd = new CWindow(hInst, NULL, "Caption");
	mainWnd.Show();
	mainWnd.Update();

	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
	    TranslateMessage(&Msg);
	    DispatchMessage(&Msg);
	}
}
```  

## How use it

To use it, you only have to clone *CWindow* project in a local folder and use that as a base project.

You can create a new instance of *CWindow* as application main window or you can extend it to make a new window class with more funcionalities.

## Contributing

If you have a patch, or stumbled upon an issue with CWindow, you can contribute this back to the code.

## References

* [A Simple Win32 Window Wrapper Class](http://www.codeproject.com/Articles/2556/A-Simple-Win-Window-Wrapper-Class) by Jason Henderson.
* [Win32 Window Wrapper Class](http://www.cpp-home.com/tutorials/285_1.htm) by Oluseyi Sonaiya.

## License

Code and documentation copyright 2014 by Jorge Soto García. Code released under the [GPLv2.0 license](http://www.gnu.org/licenses/gpl-2.0.html). Docs released under [Creative Commons Share Alike v4.0](https://creativecommons.org/licenses/by-sa/4.0/legalcode).