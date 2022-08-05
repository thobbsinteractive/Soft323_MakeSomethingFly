/* Name: d3dUtility.cpp
   Description: The methods for the d3dUtility class

   Author: Frank Luna
*/
#include "d3dUtility.h"

bool d3d::InitD3D(HINSTANCE hInstance,
				  HWND* hwnd,int width, int height) // Create main application window
{
	//
	// Create the main application window.
	//
	
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = (WNDPROC)d3d::WndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "Direct3D9App";

	if( !RegisterClass(&wc) ) 
	{
		::MessageBox(0, "RegisterClass() - FAILED", 0, 0);
		return false;
	}
		
	*hwnd = ::CreateWindow("Direct3D9App", "Direct3D9App", 
		WS_EX_TOPMOST,
		0, 0, width, height,
		0 /*parent hwnd*/, 0 /* menu */, hInstance, 0 /*extra*/); 

	if( !*hwnd )
	{
		::MessageBox(0, "CreateWindow() - FAILED", 0, 0);
		return false;
	}

	::ShowWindow(*hwnd, SW_SHOW);
	::UpdateWindow(*hwnd);
	
	return true;
}
DWORD d3d::FtoDw(float f)
{
	return *((DWORD*)&f);
}

int d3d::EnterMsgLoop( bool (*ptr_display)(float timeDelta) )
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	static float lastTime = (float)timeGetTime(); 

	while(msg.message != WM_QUIT)
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
        {	
			float currTime  = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;

			ptr_display(timeDelta);

			lastTime = currTime;
        }
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, 
							  LPARAM lParam)
{
		switch(msg)
		{
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
				::DestroyWindow(hwnd);
			break;
		}
		return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

D3DMATERIAL9 d3d::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient  = a;
	mtrl.Diffuse  = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power    = p;
	return mtrl;
}