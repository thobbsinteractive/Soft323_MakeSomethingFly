//////////////////////////////////////////////////////////////////////////////////////////////////
// seaSurface.h
// Author: Tim Hobbs
// Description: A subclass of frank luna's surface class, this adds sound and a wave effect to it.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef INC_seaSurface_H
#define INC_seaSurface_H

#include "d3dUtility.h"
#include "surface.h"
#include "animatedTexture.h"
#include <math.h>
#include "dsutil.h" 

class SeaSurface : public Surface
{
public:
	SeaSurface();
	~SeaSurface();
	void setupAnimatedTextures();
	void draw(float timeDelta);
	void wave(float timeDelta);
	void calculateWave(float timeDelta);
	void setObjectSound(CSound* _sound);
	void playSound();

private:
	animatedTexture texture;
	float angle;
	float firstangle;
	float waveHeight;
	float waveFrequency;
	float waveSpeed;
	float wavetime;
	bool goingUp;
	CSound* sound;
};

#endif //INC_seaSurface_H