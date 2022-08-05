//////////////////////////////////////////////////////////////////////////////////////////////////
// fire.h
// Author: Tim Hobbs
// Description: models the behaviors of the fireballs
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef INC_fire_H
#define INC_fire_H

#include "d3dUtility.h"
#include "objectTransform.h"

class fire : public objectTransform
{
public:
	fire(float _health,D3DXVECTOR3* _pos, 
			D3DXVECTOR3* _rotation, 
			D3DXVECTOR3* _look);
	~fire();

	float getHealth();
	void setHealth(float _health);
	void updateHealth(float timeDelta);
	void walk(float units);
	void getMatrix(D3DXMATRIX* V, D3DXVECTOR3 _rotation);
private:
	float health;
	float speed;
};

#endif //INC_fire_H