//////////////////////////////////////////////////////////////////////////////////////////////////
// light.h
// Author: Tim Hobbs / Frank Luna
// Description: I really just put Franks code in a class for this one.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __light__
#define __light__

#include "d3dUtility.h"

class Light
{
	public:
		Light();
		~Light();
		D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
		D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
		D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);
};
#endif //__light__
