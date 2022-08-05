//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: pSystem.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Represents a geneal particle system.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include "pSystem.h"

const DWORD Particle::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

PSystem::PSystem()
{
	localDevice = 0;
	vb     = 0;
	tex    = 0;
}

PSystem::~PSystem()
{
	d3d::Release<IDirect3DVertexBuffer9*>(vb);
	d3d::Release<IDirect3DTexture9*>(tex);
}

bool PSystem::init(IDirect3DDevice9* device, char* texFileName)
{
	// vertex buffer's size does not equal the number of particles in our system.  We
	// use the vertex buffer to draw a portion of our particles at a time.  The arbitrary
	// size we choose for the vertex buffer is specified by the _vbSize variable.

	localDevice = device; // save a ptr to the device

	HRESULT hr = 0;

	hr = localDevice->CreateVertexBuffer(
		vbSize * sizeof(Particle),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		Particle::FVF,
		D3DPOOL_DEFAULT, // D3DPOOL_MANAGED can't be used with D3DUSAGE_DYNAMIC 
		&vb,
		0);
	
	if(FAILED(hr))
	{
		::MessageBox(0, L"CreateVertexBuffer() - FAILED", L"PSystem", 0);
		return false;
	}

	hr = D3DXCreateTextureFromFileA(
		device,
		texFileName,
		&tex);

	if(FAILED(hr))
	{
		::MessageBox(0, L"D3DXCreateTextureFromFile() - FAILED", L"PSystem", 0);
		return false;
	}

	return true;
}

void PSystem::reset()
{
	std::list<Attribute>::iterator i;
	for(i = particles.begin(); i != particles.end(); i++)
	{
		resetParticle( &(*i) );
	}
}

void PSystem::addParticle()
{
	Attribute attribute;

	resetParticle(&attribute);

	particles.push_back(attribute);
}

void PSystem::preRender()
{
	localDevice->SetRenderState(D3DRS_LIGHTING, false);
	localDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	localDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true); 
	localDevice->SetRenderState(D3DRS_POINTSIZE, d3d::FtoDw(size));
	localDevice->SetRenderState(D3DRS_POINTSIZE_MIN, d3d::FtoDw(0.0f));

	// control the size of the particle relative to distance
	localDevice->SetRenderState(D3DRS_POINTSCALE_A, d3d::FtoDw(0.0f));
	localDevice->SetRenderState(D3DRS_POINTSCALE_B, d3d::FtoDw(0.0f));
	localDevice->SetRenderState(D3DRS_POINTSCALE_C, d3d::FtoDw(1.0f));
		
	// use alpha from texture
	localDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	localDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	localDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	localDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    localDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void PSystem::postRender()
{
	localDevice->SetRenderState(D3DRS_LIGHTING,          true);
	localDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	localDevice->SetRenderState(D3DRS_POINTSCALEENABLE,  false);
	localDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,  false);
}

void PSystem::render()
{
	//
	// Remarks:  The render method works by filling a section of the vertex buffer with data,
	//           then we render that section.  While that section is rendering we lock a new
	//           section and begin to fill that section.  Once that sections filled we render it.
	//           This process continues until all the particles have been drawn.  The benifit
	//           of this method is that we keep the video card and the CPU busy.  

	if( !particles.empty() )
	{
		//
		// set render states
		//

		preRender();
		
		localDevice->SetTexture(0, tex);
		localDevice->SetFVF(Particle::FVF);
		localDevice->SetStreamSource(0, vb, 0, sizeof(Particle));

		//
		// render batches one by one
		//

		// start at beginning if we're at the end of the vb
		if(vbOffset >= vbSize)
			vbOffset = 0;

		Particle* v = 0;

		vb->Lock(
			vbOffset    * sizeof( Particle ),
			vbBatchSize * sizeof( Particle ),
			(void**)&v,
			vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;

		//
		// Until all particles have been rendered.
		//
		std::list<Attribute>::iterator i;
		for(i = particles.begin(); i != particles.end(); i++)
		{
			if( i->isAlive )
			{
				//
				// Copy a batch of the living particles to the
				// next vertex buffer segment
				//
				v->position = i->position;
				v->color    = (D3DCOLOR)i->color;
				v++; // next element;

				numParticlesInBatch++; //increase batch counter

				// if this batch full?
				if(numParticlesInBatch == vbBatchSize) 
				{
					//
					// Draw the last batch of particles that was
					// copied to the vertex buffer. 
					//
					vb->Unlock();

					localDevice->DrawPrimitive(
						D3DPT_POINTLIST,
						vbOffset,
						vbBatchSize);

					//
					// While that batch is drawing, start filling the
					// next batch with particles.
					//

					// move the offset to the start of the next batch
					vbOffset += vbBatchSize; 

					// don't offset into memory thats outside the vb's range.
					// If we're at the end, start at the beginning.
					if(vbOffset >= vbSize) 
						vbOffset = 0;       

					vb->Lock(
						vbOffset    * sizeof( Particle ),
						vbBatchSize * sizeof( Particle ),
						(void**)&v,
						vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0; // reset for new batch
				}	
			}
		}

		vb->Unlock();

		// its possible that the LAST batch being filled never 
		// got rendered because the condition 
		// (numParticlesInBatch == _vbBatchSize) would not have
		// been satisfied.  We draw the last partially filled batch now.
		
		if( numParticlesInBatch )
		{
			localDevice->DrawPrimitive(
				D3DPT_POINTLIST,
				vbOffset,
				numParticlesInBatch);
		}

		// next block
		vbOffset += vbBatchSize; 

		//
		// reset render states
		//

		postRender();
	}
}

bool PSystem::isEmpty()
{
	return particles.empty();
}

bool PSystem::isDead()
{
	std::list<Attribute>::iterator i;
	for(i = particles.begin(); i != particles.end(); i++)
	{
		// is there at least one living particle?  If yes,
		// the system is not dead.
		if( i->isAlive )
			return false;
	}
	// no living particles found, the system must be dead.
	return true;
}

void PSystem::removeDeadParticles()
{
	std::list<Attribute>::iterator i;

	i = particles.begin();

	while( i != particles.end() )
	{
		if( i->isAlive == false )
		{
			// erase returns the next iterator, so no need to
		    // incrememnt to the next one ourselves.
			i = particles.erase(i); 
		}
		else
		{
			i++; // next in list
		}
	}
}

float PSystem::getRandomFloat(float lowBound, float highBound)
{
	if( lowBound >= highBound ) // bad input
		return lowBound;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f; 

	// return float in [lowBound, highBound] interval. 
	return (f * (highBound - lowBound)) + lowBound; 
}

void PSystem::getRandomVector(
	  D3DXVECTOR3* out,
	  D3DXVECTOR3* min,
	  D3DXVECTOR3* max)
{
	out->x = getRandomFloat(min->x, max->x);
	out->y = getRandomFloat(min->y, max->y);
	out->z = getRandomFloat(min->z, max->z);
}

// My Snow one generates a random vector according to a location
void PSystem::getRandomVector(
	  D3DXVECTOR3* out,
	  D3DXVECTOR3* min,
	  D3DXVECTOR3* max,
	  D3DXVECTOR3* pos)
{
	out->x = getRandomFloat(min->x+pos->x, max->x+pos->x);
	out->y = getRandomFloat(min->y+pos->y, max->y+pos->y);
	out->z = getRandomFloat(min->z+pos->z, max->z+pos->z);
}

void PSystem::updateBoundingBoxPosition(float _x, float _y, float _z)
{
	boundingBox.pos.x = _x;
	boundingBox.pos.y = _y;
	boundingBox.pos.z = _z;
}