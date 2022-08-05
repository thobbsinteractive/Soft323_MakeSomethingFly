#include "splash.h"

splash::splash(D3DXVECTOR3 _pos,CSound* _sound)
{
	pos   = _pos;
	rotation.y = 0.0f;
	rotation.z = 0.0f;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f; 
	// return float in [lowBound, highBound] interval. 
	f = f * (2.0f - 0.0f) + 0.0f; 
	rotation.x = f;

	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	health = 100.0f;
	scaleSpeed = 1.0f;
	waveHeight = 10.0f;
	angle = 0.0f;
	goingUp = true;

	this->addObjectSound(_sound);
	this->playSound(0);
};

splash::~splash()
{

};

float splash::getHealth()
{
	return health;
}
void splash::setHealth(float _health)
{
	health = _health;
};

void splash::splashHeight(float timeDelta)
{
	if (goingUp == true)
	{
		if(angle < 2.0f)
		{
			//Ensure angle never goes above 2.0f
			if(angle + (scaleSpeed *timeDelta) < 2.0f)
			{
				angle = angle + (scaleSpeed *timeDelta);
			}else
			{
				angle = 2.0f;
				goingUp = false;
			}
		}
	}

	if (goingUp == false)
	{
		if(angle > 0.0f)
		{
			//Ensure angle never goes below 0.0f
			if(angle - (scaleSpeed *timeDelta) > 0.0f)
			{
				angle = angle - (scaleSpeed *timeDelta);
			}else
			{
				angle = 0.0f;
				//kill it once its peaked once
				health = 0.0f;
			}
		}
	}
}

void splash::behaviour(float timeDelta)
{
	scale.x = sinf(angle) * waveHeight;
	scale.y = sinf(angle) * waveHeight;
	scale.z = sinf(angle) * waveHeight;

	splashHeight(timeDelta);
};
