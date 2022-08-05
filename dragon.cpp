#include "dragon.h"

dragon::dragon()
{
	health = 100.0f;
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	speed = 0.0f;
	speedMax = 5.0f;
	currentSpeedSetting = 0.0f;
	leanY = 0.0f;
	fireRate = 0.25f;
	timeTillFire = 0.0f;
}

dragon::dragon(float _health,D3DXVECTOR3 _pos, 
				D3DXVECTOR3 _rotation, 
				D3DXVECTOR3 _look)
{
	pos = _pos;
	health = _health;
	rotation = _rotation;
	look = _look;

	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	speed = 0.0f;
	speedMax = 5.0f;
	currentSpeedSetting = 0.0f;
	accelerationRate = 10.0f; // The Lower the faster
	leanY = 0.0f;
	fireRate = 0.25f;
	timeTillFire = 0.0f;

};
dragon::~dragon()
{

};

void dragon::setHealth(float _health)
{
	health = _health;
};

void dragon::addLimb(float _health,float _speed,
					float _movementRotationUpper,
					float _movementRotationLower,
					bool _rotateUp,
					char _rotationAxis,
					D3DXVECTOR3* _pos, 
					D3DXVECTOR3* _rotation, 
					D3DXVECTOR3* _look,
					D3DXVECTOR3* _scale)
{
	limb temp(100.0f,_speed,_movementRotationUpper,
				_movementRotationLower,_rotateUp,_rotationAxis,
				_pos, _rotation, _look, _scale);
	limbs.push_back(temp);
};

void dragon::addLimb(float _health,float _speed,
					float _movementRotationUpper,
					float _movementRotationLower,
					bool _rotateUp,
					char _rotationAxis)
{
	limb temp(100.0f,_speed,_movementRotationUpper,_movementRotationLower,_rotateUp, _rotationAxis,&pos,&rotation,&look,&scale);
	limbs.push_back(temp);
};

void dragon::addLimb(float _health,float _speed,
					float _movementRotationUpper,
					float _movementRotationLower,
					bool _rotateUp,
					char _rotationAxis,
					CSound* _limbSound)
{
	limb temp(100.0f,_speed,_movementRotationUpper,_movementRotationLower,_rotateUp, _rotationAxis,&pos,&rotation,&look,&scale,&*_limbSound);
	limbs.push_back(temp);
};

void dragon::shoot()
{
	if (timeTillFire <= 0.0f)
	{
		if (fireBalls.size() < 20)
		{
			fire temp(100.0f,&pos,&rotation,&look);
			fireBalls.push_back(temp);
			playSound(0);
		}
		timeTillFire = fireRate;
	}
}

void dragon::updateFire(float timeDelta)
{
D3DXVECTOR3 splashpos;

	for(int i = 0; i < (int)fireBalls.size(); i++)
	{
		fireBalls[i].walk(timeDelta);
		fireBalls[i].updateHealth(timeDelta);

		if (fireBalls[i].getHealth() < 0.0f)
		{
			//Delete fire ball if its dead
			fireBalls.erase(fireBalls.begin() + i);
		}

		// check fireball doesn't hit the water (me thinks all this should really be done in fireball)
		if (fireBalls[i].getPosY() <= 0.0f)
		{
			//splash

			splashpos = D3DXVECTOR3(fireBalls[i].getPosX(), fireBalls[i].getPosY(), fireBalls[i].getPosZ());
			splash temp(splashpos,sound[1]);
			splashes.push_back(temp);

			fireBalls.erase(fireBalls.begin() + i);
			
			// splash
		}
	}
	timeTillFire = timeTillFire - timeDelta;
}

void dragon::updateSplash(float timeDelta)
{
	for(int i = 0; i < (int)splashes.size(); i++)
	{
		splashes[i].behaviour(timeDelta);

		// If its dead delete it
		if(splashes[i].getHealth() <= 0.0f)
		{
			splashes.erase(splashes.begin() + i);
		}
	}
}

void dragon::update(float timeDelta)
{
	// Maintain objects speed
	walk(speed * timeDelta,false);
	updateFire(timeDelta);
	updateSplash(timeDelta);
}

void dragon::getFireMatrix(int i,D3DXMATRIX* V)
{
	fireBalls[i].getMatrix(&*V,rotation);
};

int dragon::getNumberofFireBalls()
{
	return (int)fireBalls.size();
}

void dragon::getSplashMatrix(int i,D3DXMATRIX* V)
{
	splashes[i].getMatrix(&*V);
};

int dragon::getNumberofSplashes()
{
	return (int)splashes.size();
}


void dragon::getLimbMatrix(int i,D3DXMATRIX* V,float timeDelta)
{
	D3DXVECTOR3 rotationTemp;
	rotationTemp = rotation;
	//Apply leanY here
	rotationTemp.y += leanY;
	limbs[i].getMatrix(&*V,pos,rotationTemp,look,timeDelta);
};

int dragon::getNumberofLimbs()
{
	return (int)limbs.size();
}

float dragon::getCurrentSpeedSetting()
{
	return currentSpeedSetting;
}
float dragon::getCurrentSpeed()
{
	return speed;
}

void dragon::walk(float units,bool keypress)
{
    // Addition of speed controls means overloading the inherted function.
	D3DXVECTOR3 newLook;
	D3DXVECTOR3 posTemp;
	D3DXMATRIX RotationMatrix;
	D3DXVECTOR3 normedVector;
	
	// If a key is press increase or reduce the currentSpeedSetting setting accordingly
	if (keypress == true)
	{
		if((units > 0.0f) && (currentSpeedSetting < speedMax))
		{
			// Ensure that the setting does not go above speedMax
			if(currentSpeedSetting + units < speedMax)
			{
				currentSpeedSetting = currentSpeedSetting + units/2;
			}else
			{
				currentSpeedSetting = speedMax;
			}
		}
		
		if((units <= 0.0f) && (currentSpeedSetting > 0.0f))
		{
			// Ensure that the setting does not go below 0.0f
			if(currentSpeedSetting + units > 0.0f)
			{
				currentSpeedSetting = currentSpeedSetting + units/2;
			}else
			{
				currentSpeedSetting = 0.0f;
			}
		}
	}
	

	if(speed != currentSpeedSetting)
	{
		// Speed up to current speed setting if a positive value is used
		if(speed < currentSpeedSetting)
		{
			// Ensure that the setting does not go above currentSpeedSetting
			if(units > 0.0f)
			{
				if(speed + units/5 < currentSpeedSetting)
				{
					speed = speed + units/5;
				}else
				{
					speed = currentSpeedSetting;
				}
			}else
			{
				if(speed - units/5 < currentSpeedSetting)
				{
					speed = speed - units/5;
				}else
				{
					speed = currentSpeedSetting;
				}
			}
		}
		
		// Slow down to current speed setting if a negative value is used
		if(speed > currentSpeedSetting)
		{
			// Ensure that the setting does not go below 0.0f
			if(units < 0.0f)
			{
				if(speed + units/5 > 0.0f)
				{
					speed = speed + units/5;
				}else
				{
					speed = 0.0f;
				}
			}else
			{
				if(speed - units/5 > 0.0f)
				{
					speed = speed - units/5;
				}else
				{
					speed = 0.0f;
				}
			}
		}
	}


	// The "look" vector isn't even updated, if I assign the value of "normedVector" to "look"
	// The objects trajectory is messed up? normedVector seems to hold the amount of change to
	// make, not the actual vector.

	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y,rotation.z);
    D3DXVec3TransformCoord(&newLook, &look, &RotationMatrix);
    D3DXVec3Normalize(&normedVector, &newLook );

	posTemp.x = pos.x;
	posTemp.y = pos.y;
	posTemp.z = pos.z;

	posTemp += normedVector * speed;

	//Test the new position first, if its okay update
	if((posTemp.x < objectBoundaries.Max.x) && (posTemp.x > objectBoundaries.Min.x))
	{
		pos.x = posTemp.x;
	}
		
	if((posTemp.y < objectBoundaries.Max.y) && (posTemp.y > objectBoundaries.Min.y))
	{
		pos.y = posTemp.y;
	}
		
	if((posTemp.z < objectBoundaries.Max.z) && (posTemp.z > objectBoundaries.Min.z))
	{
		pos.z = posTemp.z;
	}

	// Speed up or slow down limbs
	for(int i = 0; i < (int)limbs.size();i++)
	{
		limbs[i].setSpeed(speed);
	}

	// Tilt the Dragon back slightly if it is at a slow speed

	if (speed/10.0f < 0.5f)
	{
		leanBack(-0.5f + speed/10.0f);
	}
}

void dragon::leanBack(float angle)
{
	leanY = angle;
}

void dragon::yaw(float angle)
{
	//angle = angle * (speed/10.0f);

	float twoPi = D3DX_PI*2;

	if (angle > twoPi)
		angle -= twoPi;         //Just to stop the rotation angle

	if (angle < 0) 
		angle += twoPi;

	rotation.x = rotation.x  + angle;
}

void dragon::getMatrix(D3DXMATRIX* V)
{
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX Translation;
	D3DXMATRIX Scaling;

	D3DXMatrixScaling(&Scaling,1.0f,1.0f,1.0f);
	// Add in the lean here so as not to effect the object lookat values
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y+leanY,rotation.z);
	D3DXMatrixTranslation(&Translation, pos.x,pos.y,pos.z);

	*V = RotationMatrix * Translation * Scaling;
};