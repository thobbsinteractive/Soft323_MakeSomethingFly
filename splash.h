//////////////////////////////////////////////////////////////////////////////////////////////////
// splash.h
// Author: Tim Hobbs
// Description: Models splash created when a fireball hits the water
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef INC_splash_H
#define INC_splash_H

#include "d3dUtility.h"
#include "objectTransform.h"

class splash : public objectTransform
{
public:
	splash(D3DXVECTOR3 _pos,CSound* _sound);
	~splash();

	void updateHealth(float timeDelta);
	float getHealth();
	void setHealth(float _health);
	void behaviour(float timeDelta);
	void splashHeight(float timeDelta);
private:
	float health;
	float scaleSpeed;
	float angle;
	float waveHeight;
	bool goingUp;
};

#endif //INC_splash_H