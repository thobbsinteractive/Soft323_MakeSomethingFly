//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: boundingObjects.h
// Author: Frank Luna
// Description: Used for the snow partical effect
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <limits>

struct BoundingBox
{
	BoundingBox()
	{
		// infinite small 
		min.x = FLT_MAX;
		min.y = FLT_MAX;
		min.z = FLT_MAX;

		max.x = -FLT_MAX;
		max.y = -FLT_MAX;
		max.z = -FLT_MAX;

		pos.x = 0.0f;
		pos.y = 0.0f;
		pos.z = 0.0f;
	}

	bool isPointInside(D3DXVECTOR3& p)
	{
		// modified by me to allow for movement through the snow
		if( (p.x >= (pos.x + min.x)) && (p.y >= (pos.y + min.y)) && (p.z >= (pos.z + min.z)) &&
			(p.x <= (pos.x + max.x)) && (p.y <= (pos.y + max.y)) && (p.z <= (pos.z + max.z)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	D3DXVECTOR3 min;
	D3DXVECTOR3 max;
	D3DXVECTOR3 pos;
};

struct BoundingSphere
{
	BoundingSphere();

	D3DXVECTOR3 center;
	float       radius;
};