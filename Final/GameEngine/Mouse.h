#ifndef MOUSE
#define MOUSE

#include <algorithm>
#include <Windows.h>
#include <WindowsX.h>

#include "InputDevice.h" 
#include "Utility.h" 

//For using WM_INPUT high-res mouse data
#ifndef HID_USAGE_PAGE_GENERIC
	#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
	#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif
#define RAWHEADSIZE sizeof(RAWINPUTHEADER)

//For easily specifying a particular button
enum class MouseButton
{
	LBUTTON,
	RBUTTON,
	MBUTTON,
	BUTTON4,
	BUTTON5 
};
 
/*
A Mouse object represents the current state of the mouse and cursor
*/
class Mouse : public InputDevice
{
public:
	Mouse(float snstvty = 10);
	void InitMouseCallback(const HWND &windowHandle);
	void MouseCallback(WPARAM wParam, LPARAM lParam);
	void ClipMouse(const HWND &windowHandle);
	void onFocus(const HWND &windowHandle) override;
	void LockMouse(bool lock, const HWND &windowHandle);
	void EndFrame();
	
	//return whether a button has just been clicked
	inline bool ButtonPressed(MouseButton button) const
	{
		return buttons[(int)button] && !prevButtons[(int)button];
	}

	//return whether a button is currently down
	inline bool ButtonDown(MouseButton button) const
	{ 
		return buttons[(int)button];
	}

	//Return whether a button has just been released
	inline bool ButtonReleased(MouseButton button) const
	{
		return !buttons[(int)button] && prevButtons[(int)button];
	}

	//Return whether a button has been double clicked
	inline bool ButtonDblClicked(MouseButton button) const
	{
		return dblButtons[(int)button];
	}
	 
	//Return how much the mouse moved on the x axis since the last frame
	inline float DX() const
	{
		return dx;
	}

	//Return how much the mouse moved on the y axis since the last frame
	inline float DY() const
	{
		return dy;
	}

	//Return how much the wheel has been scrolled since the last frame
	inline int WheelDX() const
	{
		return wheelDX;
	} 

	//Get the current mouse position relative to the game window
	inline XMFLOAT2 Pos() const
	{
		return pos;
	}

	//Set the new mouse position and that the mouse has been moved
	inline void OnMouseMove(LPARAM lParam)
	{
		pos.x = (float)GET_X_LPARAM(lParam);
		pos.y = (float)GET_Y_LPARAM(lParam);
		moved = true;
	}

	//Set that a button is down
	inline void OnMouseDown(MouseButton button)
	{ 
		buttons[(int)button] = true;
		pressed = true;
	}

	//Set that a button has been released
	inline void OnMouseUp(MouseButton button)
	{
		buttons[(int)button] = false;
	}

	//Set that a button has been double clicked
	inline void OnMouseDBLClick(MouseButton button)
	{
		dblButtons[(int)button] = true;
		buttons[(int)button] = true;
	}

	//Set that the mouse wheel has been scrolled
	inline void OnMouseWheel(int wheeldx)
	{
		wheelDX = wheeldx/120;//Goes in increments of 120 so I'm just scaling down to increments of 1
	}

	//Set whether the cursor is currently visible
	inline void ShowMouse(bool show)
	{
		ShowCursor(show);
	}

	//Return whether the cursor is currently bound to the game window or not
	inline bool Locked() const
	{
		return locked;
	}

	//Return whether the mouse has moved since the last frame
	inline bool Moved() const
	{
		return moved;
	}

	//Return whether the mouse has been clicked since the last frame
	inline bool Pressed() const
	{
		return pressed;
	}
	 
protected:
	RAWINPUT* raw;
	UINT dwSize;
	BYTE lpb[40];
	XMFLOAT2 pos;
	float dx, dy, sensitivity;
	int wheelDX;
	bool pressed, moved, locked, buttons[5], dblButtons[5], prevButtons[5];
};

#endif