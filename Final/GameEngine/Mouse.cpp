#include "Mouse.h"

//Constructor
Mouse::Mouse(float snstvty)
	: sensitivity(snstvty),
	raw(nullptr),
	dx(0),
	dy(0),
	dwSize(40),
	locked(false),
	moved(false),
	pressed(false),
	wheelDX(0)
{
	if (GetSystemMetrics(SM_MOUSEPRESENT))
		connected = true;

	std::fill_n(buttons, 5, false);
	std::fill_n(prevButtons, 5, false);
	std::fill_n(dblButtons, 5, false);
}

//Register mouse to get high-res input data through WM_INPUT signals
void Mouse::InitMouseCallback(const HWND &windowHandle)
{
	RAWINPUTDEVICE Rid[1];
	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	Rid[0].dwFlags = RIDEV_INPUTSINK;
	Rid[0].hwndTarget = windowHandle;
	RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
}

//Update the mouse when it receives an WM_INPUT signal
void Mouse::MouseCallback(WPARAM wParam, LPARAM lParam)
{
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
		lpb, &dwSize, RAWHEADSIZE);
	raw = (RAWINPUT*)lpb;
	if (raw->header.dwType == RIM_TYPEMOUSE)
	{	
		moved = true;
		dx = CLAMP(-sensitivity, sensitivity, (raw->data.mouse.lLastX) / sensitivity);
		dy = CLAMP(-sensitivity, sensitivity, (-raw->data.mouse.lLastY) / sensitivity); 
	}	
}

//http://msdn.microsoft.com/en-us/library/windows/desktop/ms648380(v=vs.85).aspx#_win32_Confining_a_Cursor
//This ensures the mouse is within the play space at all times.
void Mouse::ClipMouse(const HWND &windowHandle)
{
	RECT window;
	GetClientRect(windowHandle, &window);

	// Nifty trick takes advantage of the fact that RECT is 
	// {LONG,LONG,LONG,LONG} and POINT is {LONG,LONG}. 
	ClientToScreen(windowHandle, reinterpret_cast<POINT*>(&window.left));
	ClientToScreen(windowHandle, reinterpret_cast<POINT*>(&window.right));

	// Ensures the mouse is in play space. 
	SetCursorPos((window.top + window.bottom) / 2, (window.left + window.right) / 2); 

	ClipCursor(&window);
} 

//Reacquire the mouse if the window goes out of focus and back into focus
void Mouse::onFocus(const HWND &windowHandle)
{
	if (connected)
	{
		if (locked)
			ClipMouse(windowHandle);
	}
}

//Lock the mouse to the game window
//If the game is not fullscreen and you are using a cursor in the game like in a menu,
//it is preferrable to be able to leave the game window, "X" out, go to other windows, etc.
void Mouse::LockMouse(bool lock, const HWND &windowHandle)
{
	if (locked != lock)
	{
		locked = lock;
		if (locked)
			ClipMouse(windowHandle);
		else
			ClipCursor(NULL);
	}
}

//Reset at the end of a frame
void Mouse::EndFrame()
{
	std::fill_n(dblButtons, 5, false);
	wheelDX = 0;
	dx = 0;
	dy = 0;
	moved = false;
	pressed = false;
}