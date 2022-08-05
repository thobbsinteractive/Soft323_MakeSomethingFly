/* Name: objectTransform.cpp
   Description: The methods for the objectTransform class
   
   Based losley on Frank D. Luna's camera class, this class holds an
   objects current positional coordinates and can be used to
   rotate an object on its own axis according to its own positiontion information.
   This is very useful for User and AI controlled objects.
*/
#include "objectTransform.h"

objectTransform::objectTransform()
{
	pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	objectBoundaries = Boundary(D3DXVECTOR3(10000.0f, 1000.0f,10000.0f),D3DXVECTOR3(-10000.0f, 100.0f, -10000.0f));

};

objectTransform::objectTransform(D3DXVECTOR3 _pos)
{
	pos   = _pos;
	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	objectBoundaries = Boundary(D3DXVECTOR3(10000.0f, 1000.0f,10000.0f),D3DXVECTOR3(-10000.0f, 100.0f, -10000.0f));
};

objectTransform::objectTransform(Boundary _objectBoundaries)
{
	objectBoundaries = _objectBoundaries;

	pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

objectTransform::objectTransform(D3DXVECTOR3 _pos, D3DXVECTOR3 _rotation, D3DXVECTOR3 _look)
{
	D3DXVECTOR3 pos = _pos;
	D3DXVECTOR3 rotation = _rotation;
	D3DXVECTOR3 look = _look;
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	objectBoundaries = Boundary(D3DXVECTOR3(10000.0f, 1000.0f,10000.0f),D3DXVECTOR3(-10000.0f, 100.0f, -10000.0f));
};

objectTransform::~objectTransform()
{
	/*
	for(int i =0;i < (int)sound.size();i++)
	{
		//erase all sounds
		sound.erase(sound.begin() + i);
	}
	*/
};

void objectTransform::getPosition(D3DXVECTOR3* _pos)
{
	*_pos = pos;
};

void objectTransform::setPosition(D3DXVECTOR3* _pos)
{
	pos = *_pos;
};

void objectTransform::getRotation(D3DXVECTOR3* _rotation)
{
	*_rotation = rotation;
};

void objectTransform::setRotation(D3DXVECTOR3* _rotation)
{
	rotation = *_rotation;
};
void objectTransform::getLook(D3DXVECTOR3* _look)
{
	*_look = look;
};

void objectTransform::setLook(D3DXVECTOR3* _look)
{
	look = *_look;
};

void objectTransform::addObjectSound(CSound* _sound)
{
	if(_sound)
	{
		CSound* temp = _sound;
		sound.push_back(temp);
	}
};

void objectTransform::playSound(int soundNumber)
{
	if((int)sound.size() > 0)
	{
		sound[soundNumber]->Stop();
		sound[soundNumber]->Reset();
		sound[soundNumber]->Play();
	}
};

float objectTransform::getPosX()
{
	return pos.x;
};

float objectTransform::getPosY()
{
	return pos.y;
};

float objectTransform::getPosZ()
{
	return pos.z;
};


void objectTransform::walk(float units)
{
	// Mainly nicked from "thing3d", Thanks Nigel.

	D3DXVECTOR3 newLook;
	D3DXVECTOR3 posTemp;
	D3DXMATRIX RotationMatrix;
	D3DXVECTOR3 normedVector;

	posTemp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// move only on xz plane for land object
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y,rotation.z);
    D3DXVec3TransformCoord(&newLook, &look, &RotationMatrix);
    D3DXVec3Normalize( &normedVector, &newLook );

	// The "look" vector isn't even updated, if I assign the value of "normedVector" to "look"
	// The objects trejectory is messed up? normedVector seems to hold the amount of change to
	// make, not the actual vector.
	
	posTemp.x = pos.x;
	posTemp.y = pos.y;
	posTemp.z = pos.z;

	posTemp += normedVector * units;

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
}

void objectTransform::pitch(float angle)
{
	float twoPi = D3DX_PI*2;

	if (angle > twoPi)
		angle -= twoPi;         //Just to stop the rotation angle

	if (angle < 0) 
		angle += twoPi;
   
	rotation.y = rotation.y  + angle;

}

void objectTransform::yaw(float angle)
{
	float twoPi = D3DX_PI*2;

	if (angle > twoPi)
		angle -= twoPi;         //Just to stop the rotation angle

	if (angle < 0) 
		angle += twoPi;

	rotation.x = rotation.x  + angle;
}

void objectTransform::roll(float angle)
{
	rotation.z = rotation.z  + angle;
}

void objectTransform::getMatrix(D3DXMATRIX* V)
{
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX Translation;
	D3DXMATRIX Scaling;

	D3DXMatrixScaling(&Scaling,scale.x,scale.y,scale.z);
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,rotation.x,rotation.y,rotation.z);
	D3DXMatrixTranslation(&Translation, pos.x,pos.y,pos.z);

	*V = RotationMatrix * Scaling * Translation;
}