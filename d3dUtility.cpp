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
	wc.lpszClassName = L"Direct3D9App";

	if( !RegisterClass(&wc) ) 
	{
		::MessageBox(0, L"RegisterClass() - FAILED", 0, 0);
		return false;
	}
		
	*hwnd = ::CreateWindow(L"Direct3D9App", L"Direct3D9App", 
		WS_EX_TOPMOST,
		0, 0, width, height,
		0 /*parent hwnd*/, 0 /* menu */, hInstance, 0 /*extra*/); 

	if( !*hwnd )
	{
		::MessageBox(0, L"CreateWindow() - FAILED", 0, 0);
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

	static std::chrono::system_clock::time_point lastTime = std::chrono::system_clock::now();

	while(msg.message != WM_QUIT)
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
        {	
			std::chrono::system_clock::time_point currTime  = std::chrono::system_clock::now();
			std::chrono::duration<double, std::milli> timeDelta = (currTime - lastTime)*0.001f;

			ptr_display(timeDelta.count());

			std::chrono::duration<double, std::milli> renderTimeMS = std::chrono::system_clock::now() - currTime;

			//lock to 60 fps
			if (renderTimeMS.count() > 0.0 && renderTimeMS.count() < 16.0)
			{
				Sleep(16.0f - renderTimeMS.count());
			}

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