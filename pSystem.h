//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: pSystem.h
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Represents a geneal particle system.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __pSystemH__
#define __pSystemH__

#include "d3dUtility.h"
#include "particle.h"
#include "attribute.h"
#include "boundingObjects.h"

#include <list>

class PSystem
{
public:
	PSystem();
	virtual ~PSystem();

	virtual bool init(IDirect3DDevice9* device, char* texFileName);
	virtual void reset();
		
	// sometimes we don't want to free the memory of a dead particle,
	// but rather respawn it instead.
	virtual void resetParticle(Attribute* attribute) = 0;
	virtual void addParticle();

	virtual void update(float timeDelta) = 0;

	virtual void preRender();
	virtual void render();
	virtual void postRender();

	bool isEmpty();
	bool isDead();
	
	float getRandomFloat(float lowBound, float highBound);
	void getRandomVector(D3DXVECTOR3* out,D3DXVECTOR3* min,D3DXVECTOR3* max);
	void getRandomVector(D3DXVECTOR3* out,D3DXVECTOR3* min,D3DXVECTOR3* max,D3DXVECTOR3* pos);

	// added by me
	void updateBoundingBoxPosition(float _x, float _y, float _z);

protected:
	virtual void removeDeadParticles();

protected:
	IDirect3DDevice9*       localDevice;
	D3DXVECTOR3             origin;
	BoundingBox				boundingBox;
	float                   emitRate;   // rate new particles are added to system
	float                   size;       // size of particles
	IDirect3DTexture9*      tex;
	IDirect3DVertexBuffer9* vb;         // vertix buffer
	std::list<Attribute>    particles;
	int                     maxParticles; // max allowed particles system can have

	//
	// Following three data elements used for rendering the p-system efficiently
	//

	DWORD vbSize;      // size of vb
	DWORD vbOffset;    // offset in vb to lock   
	DWORD vbBatchSize; // number of vertices to lock starting at _vbOffset
};

#endif // __pSystemH__