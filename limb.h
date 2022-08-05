//////////////////////////////////////////////////////////////////////////////////////////////////
// limb.h
// Author: Tim Hobbs
// Description: Models the behaviour and sound of a limb
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef INC_limb_H
#define INC_limb_H

#include "d3dUtility.h"
#include "objectTransform.h"

class limb : public objectTransform
{
public:
	limb(float _health,float _speed,
		 float _movementRotationUpper,
		 float _movementRotationLower,
		 bool rotateUp,
		 char _rotationAxis,
				D3DXVECTOR3* _pos,
				D3DXVECTOR3* _rotation,
				D3DXVECTOR3* _look,
				D3DXVECTOR3* _scale);

	limb(float _health,float _speed,
		 float _movementRotationUpper,
		 float _movementRotationLower,
		 bool rotateUp,
		 char _rotationAxis,
				D3DXVECTOR3* _pos,
				D3DXVECTOR3* _rotation,
				D3DXVECTOR3* _look,
				D3DXVECTOR3* _scale,CSound* _limbSound);
	~limb();

	void setTarget(D3DXVECTOR3* _targetpos);
	void setHealth(float _health);
	void setSpeed(float _speed);
	void getMatrix(D3DXMATRIX* V, // Only need to return the Matrix
					D3DXVECTOR3 _pos, 
					D3DXVECTOR3 _rotation, 
					D3DXVECTOR3 _look,
					float timeDelta);
private:
	void moveLimb(float timeDelta);
	float health;
	float speed;
	float minimumSpeed;
	float maximumSpeed;
	D3DXVECTOR3 targetpos;
	float movementRotationUpper;
	float movementRotationLower;
	float movement;
	bool rotateUp;
	char rotationAxis;

};
#endif //INC_limb_H