//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: attribute.h
// Author: Frank Luna
// Description: used for the attributes of particals
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"

#ifndef __attribute__
#define __attribute__

struct Attribute
{
	Attribute()
	{
		lifeTime = 0.0f;
		age      = 0.0f;
		isAlive  = true;
	}

	D3DXVECTOR3 position;     
	D3DXVECTOR3 velocity;     
	D3DXVECTOR3 acceleration; 
	float       lifeTime;     // how long the particle lives for before dying  
	float       age;          // current age of the particle  
	D3DXCOLOR   color;        // current color of the particle   
	D3DXCOLOR   colorFade;    // how the color fades with respect to time
	bool        isAlive;    
};

#endif //__attribute__