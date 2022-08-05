#include "fire.h"

fire::fire(float _health,D3DXVECTOR3* _pos, 
				D3DXVECTOR3* _rotation, 
				D3DXVECTOR3* _look)
{
	pos   = *_pos;
	rotation = *_rotation;
	look = * _look;
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	health = 100.0f;
	speed = 4000.0f;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f; 

	// return float in [lowBound, highBound] interval. 
	f = f * (2.0f - 0.0f) + 0.0f; 

	rotation.z = f;
};

fire::~fire()
{

};

float fire::getHealth()
{
	return health;
}
void fire::setHealth(float _health)
{
	health = _health;
};

void fire::updateHealth(float timeDelta)
{
	health = health - (timeDelta * 10);
};

void fire::walk(float units)
{
	units = units * speed;
    // Addition of speed controls means overloading the inherted function.
	D3DXVECTOR3 newLook;
	D3DXMATRIX RotationMatrix;
	D3DXVECTOR3 normedVector;

	// The "look" vector isn't even updated, if I assign the value of "normedVector" to "look"
	// The objects trajectory is messed up? normedVector seems to hold the amount of change to
	// make, not the actual vector.

	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y,rotation.z);
    D3DXVec3TransformCoord(&newLook, &look, &RotationMatrix);
    D3DXVec3Normalize(&normedVector, &newLook );

	pos += normedVector * units;
}

void fire::getMatrix(D3DXMATRIX* V,
					D3DXVECTOR3 _rotation)
{

	D3DXMATRIX RotationMatrix;
	D3DXMATRIX Translation;
	D3DXMATRIX Scaling;

	D3DXMatrixScaling(&Scaling,scale.x,scale.y,scale.z);
	// use dragons rotation coordinates to keep the sprite facing the camera
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,_rotation.x,_rotation.y,rotation.z);
	D3DXMatrixTranslation(&Translation, pos.x,pos.y,pos.z);

	*V = RotationMatrix * Scaling * Translation ;
};