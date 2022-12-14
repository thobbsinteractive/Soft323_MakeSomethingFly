//////////////////////////////////////////////////////////////////////////////////////////////////
// particle.h
// Author: Frank Luna
// Description: A structure for a particle
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

struct Particle
{
		D3DXVECTOR3 position;
		D3DCOLOR    color;
		static const DWORD FVF;
};