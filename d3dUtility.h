//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: d3dUtility.h
// Author: Frank Luna
// Description: The header file for the d3dUtility class
//          
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef INC_D3DUTILITY_H
#define INC_D3DUTILITY_H

//#pragma unmanaged

#include <d3dx9.h>
#include <stdio.h>
#include <string>
#include <vector>

//#pragma managed

namespace d3d
{
	bool InitD3D(HINSTANCE hInstance,
				  HWND* hwnd,
				  int width, 
				  int height); // Create main application window

	int EnterMsgLoop(bool (*ptr_display)(float timeDelta));

	LRESULT CALLBACK WndProc(
			HWND hwnd,
			UINT msg,
			WPARAM wParam,
			LPARAM lParam);

	template<class T> void Release(T t)
	{
		if(t)
		{
			t->Release();
			t = 0;
		}
	}

	template<class T> void Delete(T t)
	{
		if(t)
		{
			delete t;
			t = 0;
		}
	}
	
	const D3DXCOLOR      WHITE( D3DCOLOR_XRGB(255, 255, 255) );
	const D3DXCOLOR      BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
	const D3DXCOLOR        RED( D3DCOLOR_XRGB(255,   0,   0) );
	const D3DXCOLOR      GREEN( D3DCOLOR_XRGB(  0, 255,   0) );
	const D3DXCOLOR       BLUE( D3DCOLOR_XRGB(  0,   0, 255) );
	const D3DXCOLOR     YELLOW( D3DCOLOR_XRGB(255, 255,   0) );
	const D3DXCOLOR       CYAN( D3DCOLOR_XRGB(  0, 255, 255) );
	const D3DXCOLOR    MAGENTA( D3DCOLOR_XRGB(255,   0, 255) );

	//
	// Materials
	//

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	const D3DMATERIAL9 WHITE_MTRL  = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
	const D3DMATERIAL9 RED_MTRL    = InitMtrl(RED, RED, RED, BLACK, 2.0f);
	const D3DMATERIAL9 GREEN_MTRL  = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
	const D3DMATERIAL9 BLUE_MTRL   = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);

	//
	// Conversion
	//
	DWORD FtoDw(float f);
}

#endif // INC_D3DUTILITY_H
