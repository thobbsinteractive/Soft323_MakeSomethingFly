#include "limb.h"

limb::limb(float _health,float _speed,
		   float _movementRotationUpper,
		   float _movementRotationLower,
		   bool _rotateUp,
		   char _rotationAxis,
				D3DXVECTOR3* _pos,
				D3DXVECTOR3* _rotation,
				D3DXVECTOR3* _look,
				D3DXVECTOR3* _scale)
{
	health = _health;
	speed = _speed;
	minimumSpeed = _speed;
	maximumSpeed = 10 * minimumSpeed; // to keep speeds in relation to their initial speeds
	movementRotationUpper = _movementRotationUpper;
	movementRotationLower = _movementRotationLower;

	movement = 0.0f;
	pos   = *_pos;
	rotation = *_rotation;
	look  = *_look;
	scale = *_scale;
	rotateUp = _rotateUp;
	rotationAxis = _rotationAxis;
};

limb::limb(float _health,float _speed,
		   float _movementRotationUpper,
		   float _movementRotationLower,
		   bool _rotateUp,
		   char _rotationAxis,
				D3DXVECTOR3* _pos,
				D3DXVECTOR3* _rotation,
				D3DXVECTOR3* _look,
				D3DXVECTOR3* _scale,
				CSound* _limbSound)
{
	health = _health;
	speed = _speed;
	minimumSpeed = _speed;
	maximumSpeed = 10 * minimumSpeed; // to keep speeds in relation to their initial speeds
	movementRotationUpper = _movementRotationUpper;
	movementRotationLower = _movementRotationLower;

	movement = 0.0f;
	pos   = *_pos;
	rotation = *_rotation;
	look  = *_look;
	scale = *_scale;
	rotateUp = _rotateUp;
	rotationAxis = _rotationAxis;
	addObjectSound(_limbSound);
};

limb::~limb()
{

};

void limb::setTarget(D3DXVECTOR3* _targetpos)
{
	targetpos = *_targetpos;
};
void limb::setHealth(float _health)
{
	health = _health;
};

void limb::setSpeed(float _speed)
{
	if(speed < maximumSpeed)
	{
		speed = _speed;
	}

	if (_speed < minimumSpeed)
	{
		speed = minimumSpeed;
	}

	if (_speed > maximumSpeed)
	{
		speed = maximumSpeed;
	}
};

void limb::moveLimb(float timeDelta)
{
	if (rotateUp == false)
	{
		//Ensure it does not go out of bounds
		if(movement + (speed*timeDelta) > movementRotationUpper)
		{
			movement = movementRotationUpper;
		}else
		{
			movement = movement + (speed*timeDelta);
		}

		if(movement >= movementRotationUpper)
		{
			rotateUp=true;
		}
	}else
	{	
		//Ensure it does not go out of bounds
		if(movement - (speed*timeDelta) < movementRotationLower)
		{
			movement = movementRotationLower;
		}else
		{
			movement = movement - (speed*timeDelta);
		}

		if(movement <= movementRotationLower)
		{
			rotateUp=false;
			//Play sound when limb changes direction
			playSound(0);
		}
	}

	switch(rotationAxis)
	{
			case 'y': this->yaw(movement); break;
			case 'p': this->pitch(movement); break;
			case 'r': this->roll(movement); break;
	}
};

void limb::getMatrix(D3DXMATRIX* V,
					D3DXVECTOR3 _pos, 
					D3DXVECTOR3 _rotation, 
					D3DXVECTOR3 _look,
					float timeDelta)
{

	pos = _pos;
	rotation = _rotation;

	moveLimb(timeDelta);

	D3DXMATRIX RotationMatrix;
	D3DXMATRIX Translation;
	D3DXMATRIX Scaling;

	D3DXMatrixScaling(&Scaling,scale.x,scale.y,scale.z);
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y,rotation.z);
	D3DXMatrixTranslation(&Translation, pos.x,pos.y,pos.z);

	*V = RotationMatrix * Translation * Scaling;
};