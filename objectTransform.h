//////////////////////////////////////////////////////////////////////////////////////////////////
// objectTransform.h
// Author: Tim Hobbs
// Description: a basic super class for all objects in the world, holding their positions and sound
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef INC_OBJECTTRANSFORM_H
#define INC_OBJECTTRANSFORM_H

#include "d3dUtility.h"
#include "boundary.h"
#include "dsutil.h" 

class objectTransform
{
public:
	objectTransform();
	objectTransform(D3DXVECTOR3 _pos);
	objectTransform(D3DXVECTOR3 _pos, D3DXVECTOR3 _rotation, D3DXVECTOR3 _look);
	objectTransform(Boundary _objectBoundaries);
	~objectTransform();
	
	void getPosition(D3DXVECTOR3* _pos);
	void setPosition(D3DXVECTOR3* _pos);
	void getRotation(D3DXVECTOR3* _rotation);
	void setRotation(D3DXVECTOR3* _rotation);
	void getLook(D3DXVECTOR3* _look); //
	void setLook(D3DXVECTOR3* _look);
	virtual void walk(float units); // forward/backward
	void pitch(float angle); // rotate on left vector
	void yaw(float angle); // rotate on up vector
	void roll(float angle); // rotate on look vector
	virtual void getMatrix(D3DXMATRIX* V);
	void addObjectSound(CSound* _sound);
	void playSound(int soundNumber);
	float getPosX();
	float getPosY();
	float getPosZ();

protected:
	D3DXVECTOR3 pos;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 look; //Does it even need this any more?
	D3DXVECTOR3 rotation;
	Boundary    objectBoundaries;
	std::vector<CSound*> sound;  //can have a number of associated sounds
};
#endif // INC_OBJECTTRANSFORM_H