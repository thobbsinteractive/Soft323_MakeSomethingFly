//////////////////////////////////////////////////////////////////////////////////////////////////
// dragon.h
// Author: Tim Hobbs
// Description: Models the dragons position, limb position, behaviours and fireballs
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef INC_dragon_H
#define INC_dragon_H

#include "d3dUtility.h"
#include "objectTransform.h"
#include "fire.h"
#include "splash.h"
#include "limb.h"

using std::vector;

class dragon : public objectTransform
{
public:
	dragon();
	dragon(float _health,D3DXVECTOR3 _pos, 
			D3DXVECTOR3 _rotation, 
			D3DXVECTOR3 _look);
	~dragon();

	void setHealth(float _health);
	void addLimb(float _health,float _speed,
					float _movementRotationUpper,
					float _movementRotationLower,
					bool _rotateUp,
					char _rotationAxis,
					D3DXVECTOR3* _pos, 
					D3DXVECTOR3* _rotation, 
					D3DXVECTOR3* _look,
					D3DXVECTOR3* _scale);

	void addLimb(float _health,float _speed,
					float _movementRotationUpper,
					float _movementRotationLower,
					bool _rotateUp,
					char _rotationAxis);

	void addLimb(float _health,float _speed,
					float _movementRotationUpper,
					float _movementRotationLower,
					bool _rotateUp,
					char _rotationAxis,
					CSound* _limbSound);

	void getLimbMatrix(int i,D3DXMATRIX* V,float timeDelta);
	void getFireMatrix(int i,D3DXMATRIX* V);
	int getNumberofFireBalls();
	int getNumberofLimbs();
	void getSplashMatrix(int i,D3DXMATRIX* V);
	int getNumberofSplashes();
	void getScale(D3DXVECTOR3* _scale);
	void walk(float units,bool keypress);
	void getMatrix(D3DXMATRIX* V);
	float getCurrentSpeedSetting(void);
	float getCurrentSpeed(void);
	void leanBack(float angle);
	void yaw(float angle); 
	void shoot();
	void update(float timeDelta);

private:
	void updateFire(float timeDelta);
	void updateSplash(float timeDelta);

	float health;
	D3DXVECTOR3 targetpos;
	std::vector<limb> limbs;
	std::vector<fire> fireBalls;
	std::vector<splash> splashes;
	float speed;
	float currentSpeedSetting;
	float speedMax;
	float accelerationRate;
	float leanY;
	float fireRate;
	float timeTillFire;

};

#endif //INC_dragon_H