# Simple C++ Win32 Wrapper

Usted debería entender la herencia de clases en C++, el polimorfismo, las clases abstractas, funciones virtuales y funciones virtuales puras.

Usted debería saber que este tutorial asume que tiene un completo conocimiento de C++ y un conocimiento básico de la API de Win32.

Se dará cuenta de la utilización del operador de resolución de ámbito cuando se llama a funciones de la API de Win32 como ::CreateWindowEX() o ::RegisterWindowsEX().

Eso es sólo un hábito mío cuando llamo a este tipo de funciones, básicamente dice a C++ que busque las funciones en el espacio de nombres global.

¿Le gustaría escribir programas Win32 como este?

```
MyWindowClass wndClass (hInstance, TEXT("GenericWindowClass"));
MyWindow wnd (hInstance, TEXT("My Window Title"), wndClass.className ());

wnd.Create ();
wnd.Show ();

/* message loop */
```

Todo el mundo sabe que crear una ventana no es una tarea simple pero si es bastante tediosa, así que voy a intentar hacerlo fácil. Este método es muy similar a Qt, Java, o WxWidgets. 

## El problema

Si usted nunca intentó envolver la funcionalidad de una ventana, apuesto a que la primera cosa que se preguntará es: «¿qué voy a hacer con el procedimiento de ventana?». También apuesto a que esta será la primera respuesta que cruzará su mente: «Sólo la declararé como un función miembro dentro de la clase contenedora de mi ventana y le asignaré la estructura 
> WNDClASSEX como valor para el campo *lpfnWndProc*.»

Bien, desafortunadamente no funcionará. «¿Por qué?» Porque las funciones miembro tiene un parámetro oculto adicional, el puntero *this*, así en vez de ``LRESULT CALLBACK WndProc(HWND, UNT, WPARAM, LPARAM)`` su función miembro será ``LRESULT CALLBACK WndProc(YourClassName *this, HWND, UINT, WPARAM,LPARAM)``.

Obviamente, usted no será capaz de asignar ``wcx.lpfnWndProc = memberWndProc`` porque los dos punteros a función son de tipos diferentes. 

Así cómo hacemos para solucionarlo? Usaremos una función miembro estática que actuará como un canalizador de mensajes.

Una función miembro estática puede ser invocada sin crear una instancia de la clase, por lo tanto no tiene el puntero *this* en su lista de parámetros, permitiéndonos pasarla como el miembro *lpfnWndProc* en *WNDCLASSEX*.

«¿Un encaminador de mensajes dices?», sí, porque el procedimiento de ventana va a ser un una función miembro estática, nosotros no podemos redefinir su comportamiento en una clase derivada así que nosotros vamos a usar un pequeño   hack. Vamos a declarar otra función procedimiento que será implementada en la clase derivada y hará propiamente el proceso de mensajes particular de cada ventana.

El encaminador de mensajes estático va a determinar que instancia de procedimiento de ventana será llamado asociando el manejador de ventana HWND con un puntero a la instancia de la clase derivada. Esto será explicado más tarde.

## Wrapper de ventana 

Primero nosotros crearemos una clase abstracta de ventana, la llamaremos *AbstractWindow*. Después, vamos a crear nuestra propias ventanas derivando esta clase y definiendo algunas de sus funciones miembro.

```
// This is "AbstractWindow.h"

class AbstractWindow {
        protected:
                // window handle
                HWND _hwnd;

                // ... other members ...
        public:
                AbstractWindow () {}

                // this will be WNDCLASSEX::lpfnWndProc
                static LRESULT CALLBACK msgRouter (HWND, UINT, WPARAM, LPARAM);

                // this is the actual window procedure
                // this will be implemented in derived classes and will be called by msgRouter
                virtual LRESULT CALLBACK wndProc (HWND, UINT, WPARAM, LPARAM) = 0;

                // calls CreateWindowEx, creating the window
                virtual bool Create ();
                
                // ... other member functions ...
};
```

Observe la función virtual pura *wndProc*, este es el procedimiento de ventana real que será implementado en la clase derivada, la función estática *msgRouter* sólo determina la instancia de *AbstractWindow* para enviar el menasaje y llamar a su *wndProc*. «¿Cómo?»   

En el método *AbstractWindow::Create ()*, vamos a pasar el puntero como el últomo parametro de de *CreateWindowEx()*.

Entonces, *CreateWindowEx()* le envia a *msgRouter* a través de un mensaje *WM_NCCREATE* (* **Detalles:** el puntero es guardado en el parámetro LPARAM como una estructura LPCREATESTRUC, en el campo lpCreateParams. No importante*). Nosotros entonces capturamos el mensaje *WM_NCCREATE* y asociamos el puntero a  la instancia *AbstractWindow* con el HWND de la ventana usando *SetWindowLong()*.
    
Lugo, recuperamos el puntero a la correspondiente *AbstractWindow* usando *GetWindowLong()* y llamaremos a su método *wndProc* el cual hará el proceso real.

Así que básicamente lo que va a pasar para todas las ventanas de nueva creación. Para cada una que exista, cuando los mensajes lleguen a *nsgRouter*, tendremos el manejador de ventana que usaremos para recuperar el puntero al objeto *AbstracWindow* que contiene el procedimiento de ventana, entonces reenviaremos el mensaje allí.

Aquí está el código:

```
// This is "AbstractWindow.cpp"
#include "AbstractWindow.h"

bool AbstractWindow::Create ()
{
        // we'll just assume CreateWindowEx ()'s parameters are protected members of AbstractWindow
        _hwnd = ::CreateWindowEx (
                        _styleEx,
                        _className,
                        _windowName,
                        _style,
                        _x,
                        _y,
                        _width,
                        _height,
                        _hwndParent,
                        _hMenu,
                        _hInstance,
                        this                    // pointer to this class instance
                );

        if (!_hwnd) return false;
        else return true;       
        
}
LRESULT CALLBACK AbstractWindow::msgRouter (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
{
        AbstractWindow *wnd = 0;        // pointer to the window that should receive the message

        if (message == WM_NCCREATE) {   
                // if this message gets sent then a new window has just been created, 
                // so we'll asociate its handle with its AbstractWindow instance pointer
                ::SetWindowLong (hwnd, GWL_USERDATA, long((LPCREATESTRUCT(lParam))->lpCreateParams));
        }

        // --- messages different from WN_NCCREATE / or WM_NCCREATE was just processed ---
        // we retrieve the instance of AbstractWindow that corresponds to the destination window's HWND
        wnd = (AbstractWindow *) (::GetWindowLong (hwnd, GWL_USERDATA));

        // we then route the message to the wndProc method defined in the derived AbstractWindow class
        if (wnd)
                wnd->wndProc (hwnd, message, wParam, lParam);
        else
                // for messages that arrive prior to WM_NCCREATE 
                // and the HWND <-> AbstractWindow * association was not made
                return ::DefWindowProc (hwnd, message, wParam, lParam);
}
```

Ahora la única cosa que resta es derivar una clase de *AbstractWindow* y definir el método *wndProc*. Antes de hacerlo voy a mostrarle como envolver la estructura WNDCLASSEX así puede crear facilmente una clase Ventana que use el *AbstractWindow::msgRouter* como su procedimiento de ventana.

## The Window Class Wrapper

Antes de crear una vetana primero tenemos que crear una clase Ventanta. Espero que estas alturas sepa a que me refiero con una clase Ventana. La siguiente clase C++ envolverá una clase Ventana más precisamente la estructura WNDCLASSEX, una capa por encima de la ventana envuelta.

Hay muchas formas de hacerlo, pero lo único que cada implementación debe tener en común es establecer el campo *lpfnWndProc* de *WNDCLASSEX* a *AbstractWindow::msgRouter*, porque es donde queremos colocar todos los mensajes de nuestras ventanas.

Así llamaremos a esta clase *SimpleWindowClass*. Basicamente, la única entrada que necesita el contructor de la lcase es el HINSTANCE de la aplicación y el nombre de la clase.

Este será la forma de implementarla:

```
// This is "SimpleWindowClass.h"

class SimpleWindowClass : protected WNDCLASSEX {
        public:
                SimpleWindowClass (HINSTANCE hInst, const TCHAR * className);

                // registers the class
                virtual bool Register ();
                
                //retrieve the class name
                virtual const TCHAR * className () const { return lpszClassName; }

        protected:
                // --- all WNDCLASSEX's members are protected, so they can be inherited by derived classes ---
};
```

La clase hereda de la estructura *WNDCLASSEX*, tiene una función miembro virtual que recive el nombre de la clase, una función miembro *Register()* que llama a la función miembro Win32 *RegisterClassEx()* para registrar la clase de la ventana y un constructor que rellenará la estructura *WNDCLASSEX*.

Aquí está la implementación:

```
// This is "SimpleWindowClass.cpp"
// include the AbstractWindow header, 
// we need it for the lpfnWndProc = AbstractWindow::msgRouter assignment
#include "AbstractWindow.h"     
#include "SimpleWindowClass.h"

SimpleWindowClass::SimpleWindowClass (HINSTANCE hInst, const TCHAR *className)
{
        // could've used GetModuleHandle (NULL) instead of passing the instance as a parameter
        hInstance = hInst;                              
        // all messages for windows belonging to this Window Class will get sent to msgRouter
        lpfnWndProc = AbstractWindow::msgRouter;        
        lpszClassName = className;

        // --- set default values for the rest of the WNDCLASSEX structure ---
        // --- later you can derive your own class and modify this behavior ---
        lpszMenuName = 0;
        cbSize = sizeof (WNDCLASSEX);
        cbClsExtra = 0;
        cbWndExtra = 0;
        style = 0;
        hIcon = ::LoadIcon (NULL, IDI_APPLICATION);
        hIconSm = ::LoadIcon (NULL, IDI_APPLICATION);
        hCursor = ::LoadCursor (NULL, IDC_ARROW);
        hbrBackground = (HBRUSH) ::GetStockObject (COLOR_BTNFACE);      

        // --- the constructor won't call the Register () member function ---
        // --- that was my choice, again, you can change the behavior in your code ---
}

bool SimpleWindowClass::Register ()
{
        if (::RegisterClassEx (this)) // we pass the this pointer because our class inherits from WNDCLASSEX
                return true;
        else 
                return false;
}
```

¡Magnífico! Hemos simplificado la creación de la Clase Venana de rellenar los campos de la estructura manualmente y llamar a *RegisterClassEx()* a declarar un objeto *SimpleWindowClass* y llamar a su función miembro *Register()*.

Ahora crearemos una clase *SimpleWindow* que heredará de *AbstratWindow* e implemtará la función miembro *wndProc* que hará posible el funcionamiento de nuestra ventana.

## Crear la ventana derivando AbstractWindow


Lo único que nos resta por hacer ahora es heredar de *AbstractWindow* u crear nuestro comportamiento de ventana definiendo la función virtual pura *wndProc*.

Comencemos:

```
// This is "SimpleWindow.h"
#include "AbstractWindow.h"     // include the AbstractWindow header file, needed for inheritance

class SimpleWindow : public AbstractWindow {
        public:
                // the constructor takes two arguments, the window's title, and the Window Class' name
                // the Window Class must be registered using the SimpleWindowClass object
                // you can retrieve the Window Class name using the SimpleWindowClass::className () method
                SimpleWindow (const TCHAR *windowName, const TCHAR *className);

                // this is our window's procedure, you're gonna implement it like any other window procedure
                virtual LRESULT CALLBACK wndProc (HWND, UINT, WPARAM, LPARAM);
                
                // shows the window on the screen and updates its client area
                void Show () { 
                        ::ShowWindow (_hwnd, SW_SHOW);
                        ::UpdateWindow (_hwnd);
                }
};
Now we're gonna define the constructor and the window procedure. In this case the constructor is just going to set default values for CreateWindowEx()'s parameters.

// This is "SimpleWindow.cpp"
#include "SimpleWindow.h"

SimpleWindow::SimpleWindow (const TCHAR *windowName, const TCHAR *className)
        :       AbstractWindow ()
{
        // --- we're going to assume the following members are declared in AbstractWindow as protected ---
        _windowName = windowName;
        _className = className;
        _hInstance = ::GetModuleHandle (NULL); // or you could've passed the HINSTANCE as a constructor parameter
        _style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
        _y = _x = CW_USEDEFAULT;
        _height = _width = CW_USEDEFAULT;
        _styleEx = 0;
        _hwndParent = 0;
        _hMenu = 0;
        // --- again those were supposed to be protected members of AbstractWindow --- 
}

// our window procedure, this is were we define our window's behavior
LRESULT CALLBACK SimpleWindow::wndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
        switch (message)
        {
                case WM_CREATE:
                        ::MessageBox (hwnd, TEXT("Window has been successfully created"), TEXT("Succes"), MB_OK);
                        return 0;
                case WM_DESTROY:
                        ::PostQuitMessage (0);
                        return 0;
                default:
                        return ::DefWindowProc (hwnd, message, wParam, lParam);
        }
}
By this time we've drastically reduced WinMain's size.

Your New Simple WinMain

In the new WinMain we'll just create a Window Class by creating a SimpleWindowClass object and calling its Register() member function, then we're gonna create a window by creating a SimpleWindow object, and calling its Create() and Show() member functions. Of course we're also going to need a message loop.

#include "SimpleWindowClass.h"  // create a Window Class
#include "SimpleWindow.h"       // create a window

int APIENTRY WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
        // create the Window Class
        SimpleWindowClass wndClass (hInstance, TEXT ("My window class"));
        wndClass->Register ();

        // create the window, 
        SimpleWindow wnd (TEXT ("Generic Window Title"), wndClass->className ());
        wnd->Create();

        // show the window on the screen
        wnd->Show ();
        
        // pump messages:
        MSG  msg;
        int status;
        while ((status = ::GetMessage (&msg, 0, 0, 0 )) != 0)
        {
                if (status == -1) {
                        // handle the error, break
                        break;
                }
                ::TranslateMessage (&msg);
                ::DispatchMessage (&msg);                               
        }
        return msg.wParam;
}
```

That's all there is to it. From now on there are endless possibilities. You can modify your window wrapper or your Window Class wrapper the way you want them to work.
An interesting thing you could do is add message handlers, such as OnCommand (), OnCreate (), OnPaint () and so on. You would need to define their default behavior in the AbstractWindow class and then in the msgRouter you would forward each message to its handler. Then in your derived classes you would only define handlers for the messages you'd like to handle, the rest will be handled by the base class, that is, AbstractWindow. The final result would be an MFC-like window class, with member functions for each message, this way you would get rid of the window procedure for ever.
Let me show you what I mean:

/* ---- AbstractWindow.h ---- */
class AbstractWindow {
        // --- previous class definition here ---

        public:
                virtual bool OnCommand (int ctrlId, int notifyCode) { return false; }
                virtual bool OnDestroy () { ::PostQuitMessage (0); return false; }
                virtual bool OnClose () { return false; }
                // --- and so on, add as many handlers as necessary ---
                // --- and define their default behavior ---
                // --- usually they return 0, but check MSDN for details ---
};

/* ---- AbstractWindow.cpp ---- */
LRESULT CALLBACK AbstractWindow::msgRouter (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
{
        AbstractWindow *wnd = 0;        
        
        if (message == WM_NCCREATE) {
                wnd = (AbstractWindow *) ((LPCREATESTRUCT(lParam))->lpCreateParams);
                ::SetWindowLong (hwnd, GWL_USERDATA, long(wnd));
                wnd->OnNCCreate (/* params */);
        } else {
                wnd = (AbstractWindow *) (::GetWindowLong (hwnd, GWL_USERDATA));

                if (wnd) {
                        switch (message)
                        {
                                case WM_COMMAND:
                                        return OnCommand (LOWORD (wParam), HIWORD(wParam))
                                case WM_DESTROY:
                                        return OnDestroy ();
                                case WM_CLOSE:
                                        return OnClose ();

                                // --- and so on, it'll be a lot of work to do,
                                // but most apps don't use every window message ---
                                
                                default:        // for the messages you did not define any handlers
                                        return ::DefWindowProc (hwnd, message, wParam, lParam);
                        }
                }
                else    // for messages that arrive prior to WM_NCCREATE 
                        // and the HWND -> AbstractWindow * association was not made
                        return ::DefWindowProc (hwnd, message, wParam, lParam);
        }
}
In your derived class instead of defining the old long window procedure you will only define handlers for the messages you'd like to handle, the rest of the messages will either be handled by their default handler implementation in AbstractWindow or by DefWindowProc() in msgRouter.

class SimpleWindowClass : public AbstractWindow {
        // --- rest of code here ---
        // --- constructors, private data, etc etc...
        public:
                // there is no need for a window procedure now, just add handlers.
                virtual bool OnCommand (int ctrlId, int notify Code) { /* do stuff */ }
                virtual bool OnCreate () { ::MessageBox (_hwnd, TEXT("Window created"), TEXT ("Success"), MB_OK); }
                // and so on, every message you want to handle must have a default handler in AbstractWindow 
                // and you're going to implement it here, change its behavior the way you needs
};
Enjoy!