#include "gameInput.h"

GameInput::GameInput()
{
	directInput       = NULL; // The DirectInput object         
	keyboardDevice    = NULL; // The keyboard device
	mouseDevice		  = NULL; // The mouse device

	// Mouse sensivity
	mouseXSensivity = 1.0f;
	mouseYSensivity = 1.0f;
	mouseZSensivity = 0.2f;
};

GameInput::~GameInput()
{

	if(directInput != NULL)
	{
		if(keyboardDevice != NULL)
		{
			keyboardDevice->Unacquire();
			keyboardDevice->Release();
		}

		if(mouseDevice != NULL)
		{
			mouseDevice->Unacquire();
			mouseDevice->Release();
		}
	directInput->Release();
	}
};

bool GameInput::setup_Input(HINSTANCE* mainInstance,HWND* hwnd, UserInputs* _keysPressed)
{
	bool failed = false;

	keysPressed = _keysPressed;


	// Initialise all values to false

	keysPressed->backKey = false;
	keysPressed->upKey = false;
	keysPressed->forwardKey = false;
	keysPressed->downKey = false;
	keysPressed->leftKey = false;
	keysPressed->rightKey = false;
	keysPressed->fireKey = false;
	keysPressed->escapeKey = false;

	if (FAILED(DirectInput8Create(*mainInstance, DIRECTINPUT_VERSION, 
			   IID_IDirectInput8, (void**)&directInput, NULL)))
	{
		MessageBox(NULL, "Error creating input device", "MakeSomethingFly.exe", MB_OK);
		failed = true;
	}
  
	if (FAILED(directInput->CreateDevice(GUID_SysKeyboard, &keyboardDevice, NULL)))
	{
		MessageBox(NULL, "Error creating keyboard device", "MakeSomethingFly.exe", MB_OK);
		failed = true;
	}
  
	if (FAILED(keyboardDevice->SetDataFormat(&c_dfDIKeyboard)))
    {
		MessageBox(NULL, "Error setting data format", "MakeSomethingFly.exe", MB_OK);
		failed = true;
	}
	
	if (FAILED(keyboardDevice->SetCooperativeLevel(*hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
    {
		MessageBox(NULL, "Error setting SetCooperativeLevel", "MakeSomethingFly.exe", MB_OK);
		failed = true;
	}	
  
	if (FAILED(keyboardDevice->Acquire()))
    { 
		MessageBox(NULL, "Error acquiring keyboard", "MakeSomethingFly.exe", MB_OK);
		failed = true;
	}


	// initialize the mouse
	if (FAILED(directInput->CreateDevice(GUID_SysMouse, &mouseDevice, NULL)))
	{

	}

	if (FAILED(mouseDevice->SetCooperativeLevel(*hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
	//if (FAILED(mouseDevice->SetCooperativeLevel(*hwnd, DISCL_BACKGROUND | DISCL_EXCLUSIVE)))
	{
		
	}
	if (FAILED(mouseDevice->SetDataFormat(&c_dfDIMouse)))
	{

	}
	if (FAILED(mouseDevice->Acquire()))
	{

	}

	return failed;
};

void GameInput::checkInputs()
{
	DIMOUSESTATE mouse_state; // mouse state structure

	keysPressed->backKey = false; // reset all keys (keeps them unsticky)
	keysPressed->upKey = false;
	keysPressed->forwardKey = false;
	keysPressed->downKey = false;
	keysPressed->leftKey = false;
	keysPressed->rightKey = false;
	keysPressed->fireKey = false;
	keysPressed->escapeKey = false;


	// Check the keyboard status
	if (FAILED(keyboardDevice->GetDeviceState(sizeof(unsigned char[256]), (LPVOID)keystate)))
    {	
		MessageBox(NULL, "Error getting key status", "MakeSomethingFly.exe", MB_OK);
	}

	// Check Mouse status
	if (FAILED(mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouse_state)))
	{
		MessageBox(NULL, "Error getting mouse status", "MakeSomethingFly.exe", MB_OK);
	}
	
	
	// Check changes to keyboard and mouse states
	if (KeyDown(keystate, DIK_ESCAPE))
	{
		keysPressed->escapeKey = true;
	}

	if (KeyDown(keystate, DIK_LEFT))
	{
		keysPressed->leftKey = true;
	}

	if (KeyDown(keystate, DIK_RIGHT))
	{
		keysPressed->rightKey = true;
	}

	if (KeyDown(keystate, DIK_PGUP))
	{
		keysPressed->forwardKey = true;
	}

	if (KeyDown(keystate, DIK_PGDN))
	{
		keysPressed->backKey = true;
	}

	if (KeyDown(keystate, DIK_UP))
	{
		keysPressed->upKey = true;
	}

	if (KeyDown(keystate, DIK_DOWN))
	{
		keysPressed->downKey = true;
	}

	if (KeyDown(keystate, DIK_DOWN))
	{
		keysPressed->downKey = true;
	}

	if ((KeyDown(keystate, DIK_RCONTROL)) || (KeyDown(keystate, DIK_RETURN)))
	{
		keysPressed->fireKey = true;
	}

	//Mouse overides keys

	// If left mouse button is pressed
	if (mouse_state.rgbButtons[0] & 0x80)
	{
		keysPressed->fireKey = true;
	}

	// Check for mouse movement
	if (mouse_state.lX > mouseXSensivity)
	{
		keysPressed->rightKey = true;
	}

	if (mouse_state.lX < (mouseXSensivity-mouseXSensivity-mouseXSensivity))
	{
		keysPressed->leftKey = true;
	}

	if (mouse_state.lY > mouseYSensivity)
	{
		keysPressed->downKey = true;
	}

	if (mouse_state.lY < (mouseYSensivity-mouseYSensivity-mouseYSensivity))
	{
		keysPressed->upKey = true;
	}

	if (mouse_state.lZ > mouseZSensivity)
	{
		keysPressed->forwardKey = true;
	}

	if (mouse_state.lZ < (mouseZSensivity-mouseZSensivity-mouseZSensivity))
	{
		keysPressed->backKey = true;
	}
};

