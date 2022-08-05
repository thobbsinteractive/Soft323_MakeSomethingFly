//////////////////////////////////////////////////////////////////////////////////////////////////
// snow.h
// Author: Frank Luna / Tim Hobbs
// Description: Ive Added additional snow behavior to Franks partical class. Allowing for the 
// bounding box to move with the player and for the snow to be generated at any height.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __snow__
#define __snow__

#include "attribute.h"
#include "pSystem.h"

class Snow : public PSystem
{
public:
	Snow(BoundingBox* boundingBox, int numParticles);
	void resetParticle(Attribute* attribute);
	void update(float timeDelta);
};

#endif // __snow__