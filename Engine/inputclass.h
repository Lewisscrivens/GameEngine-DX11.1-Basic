////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


//////////////
// INCLUDES //
//////////////
#include <dinput.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	void GetMouseLocation(int&, int&);
	int GetMouseX();
	int GetMouseY();
	int GetMouseXChange();
	int GetMouseYChange();
	void ResetMousePosition();

	bool IsEscapePressed();
	bool IsLeftPressed();
	bool IsRightPressed();
	bool IsUpPressed();
	bool IsDownPressed();
	bool IsWPressed();
	bool IsSPressed();
	bool IsAPressed();
	bool IsDPressed();
	bool IsMPressed();
	bool IsRPressed();
	bool IsHPressed();
	bool IsZPressed();
	bool IsLeftCtrlPressed();
	bool IsLeftShiftPressed();
	bool IsSpaceBarPressed();
	bool IsF11Pressed();
	bool IsPlusPressed();
	bool IsMinusPressed();
	bool IsNum0Pressed();
	bool IsPgUpPressed();
	bool IsPgDnPressed();
	bool Is0Pressed();
	bool Is9Pressed();


	

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY, oldMouseX, oldMouseY;
};

#endif