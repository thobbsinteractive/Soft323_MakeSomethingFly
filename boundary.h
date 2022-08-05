//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: boundary.h
// Author: Tim Hobbs
// Description: Used to determine an objects movement boundaries
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __boundaryH__
#define __boundaryH__

struct Boundary
{
	Boundary(){}
	Boundary(D3DXVECTOR3 _Max,D3DXVECTOR3 _Min) // texture coordinates
	{
		Max = _Max;
		Min = _Min;
	}
    D3DXVECTOR3 Max;
	D3DXVECTOR3 Min;
};

#endif // __boundaryH__