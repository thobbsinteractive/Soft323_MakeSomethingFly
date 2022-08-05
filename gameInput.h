//////////////////////////////////////////////////////////////////////////////////////////////////
// gameInput.h
// Author: Tim Hobbs
// Description: My own directx input class for intialising and readin keyboard and mouse inputs, 
// partly based on code by Mason Smith from Gamedev.net
//          
//////////////////////////////////////////////////////////////////////////////////////////////////


// My own directx input class for intialising and readin keyboard and mouse inputs, partly based on code
// by Mason Smith from Gamedev.net

#ifndef __gameInput__
#define __gameInput__

#include <dinput.h>
#include <windows.h>
#include "userInputs.h"
#include "d3dUtility.h"

#define KeyDown(data, n)  ((data[n] & 0x80) ? true : false)
#define KeyUp(data, n)    ((data[n] & 0x80) ? false : true)

class GameInput
{
public:
	GameInput();
	~GameInput();
	bool setup_Input(HINSTANCE* mainInstance, // A pointer to the main application instance
					 HWND* hwnd,
					 UserInputs* _keysPressed); // I get a pointer to the address of UserInputs structure so that I
											   // can update it without passing it each time in checkInputs;
	void checkInputs();

private:
	LPDIRECTINPUT8       directInput; // The DirectInput object         
	LPDIRECTINPUTDEVICE8 keyboardDevice; // The keyboard device
	LPDIRECTINPUTDEVICE8 mouseDevice; // The mouse device
	//HINSTANCE* mainInstance;
	//HWND* hwnd;
	unsigned char keystate[256]; 
	UserInputs* keysPressed;
	float mouseXSensivity;
	float mouseYSensivity;
	float mouseZSensivity;
};

#endif // __gameInput__