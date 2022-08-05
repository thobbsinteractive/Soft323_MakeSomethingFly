// A surface class with an animated texture

#include "seaSurface.h"

SeaSurface::SeaSurface()
{
	angle=-2.0f;
	waveHeight=30.0f;
	waveFrequency=1.0f;
	waveSpeed = 0.06f; // lower = faster
	wavetime = 0.0f;
	goingUp = true;
	firstangle = 0.0f;
	sound = NULL;
}

SeaSurface::~SeaSurface()
{

}
void SeaSurface::setupAnimatedTextures()
{
	texture.setupTexture(localDevice,0.1f);
	texture.addTexture("textures/water/water0001.jpg");
	texture.addTexture("textures/water/water0002.jpg");
	texture.addTexture("textures/water/water0003.jpg");
	texture.addTexture("textures/water/water0004.jpg");
	texture.addTexture("textures/water/water0005.jpg");
	texture.addTexture("textures/water/water0006.jpg");
	texture.addTexture("textures/water/water0007.jpg");
	texture.addTexture("textures/water/water0008.jpg");
	texture.addTexture("textures/water/water0009.jpg");
	texture.addTexture("textures/water/water0010.jpg");
	texture.addTexture("textures/water/water0011.jpg");
	texture.addTexture("textures/water/water0012.jpg");
	texture.addTexture("textures/water/water0013.jpg");
	texture.addTexture("textures/water/water0014.jpg");
	texture.addTexture("textures/water/water0015.jpg");
	texture.addTexture("textures/water/water0016.jpg");
	texture.addTexture("textures/water/water0017.jpg");
	texture.addTexture("textures/water/water0018.jpg");
	texture.addTexture("textures/water/water0017.jpg");
	texture.addTexture("textures/water/water0016.jpg");
	texture.addTexture("textures/water/water0015.jpg");
	texture.addTexture("textures/water/water0014.jpg");
	texture.addTexture("textures/water/water0013.jpg");
	texture.addTexture("textures/water/water0012.jpg");
	texture.addTexture("textures/water/water0011.jpg");
	texture.addTexture("textures/water/water0010.jpg");
	texture.addTexture("textures/water/water0009.jpg");
	texture.addTexture("textures/water/water0008.jpg");
	texture.addTexture("textures/water/water0007.jpg");
	texture.addTexture("textures/water/water0006.jpg");
	texture.addTexture("textures/water/water0005.jpg");
	texture.addTexture("textures/water/water0004.jpg");
	texture.addTexture("textures/water/water0003.jpg");
	texture.addTexture("textures/water/water0002.jpg");
	texture.addTexture("textures/water/water0001.jpg");
}

void SeaSurface::wave(float timeDelta)
{	


	wavetime += timeDelta;

	if(wavetime > waveSpeed)
	{
		wavetime = 0.0f;

		//too keep waves in sync store first angle
		angle=firstangle;
		calculateWave(timeDelta);
		firstangle = angle;

		Vertex* v = 0;
		vb->Lock(0, 0, (void**)&v, 0);

		float rowBeginAngle = 0;
		bool rowGoingUp = 0;

		for(int i=0; i < numVertsPerRow;i++)
		{
			rowBeginAngle = angle;
			rowGoingUp = goingUp;
			for(int j=0; j < numVertsPerCol;j++)
			{
				// compute the correct index into the vertex buffer
				// based on where we are in the nested loop.

				// update height of vertex
				v[i*(j+1)]._y = sinf(angle) * waveHeight;

				this->calculateWave(timeDelta);
			}
			angle = rowBeginAngle;
			goingUp = rowGoingUp;
			//this->calculateWave(timeDelta);
			//this->calculateWave(timeDelta);
		}
		vb->Unlock();;
	}
}

void SeaSurface::calculateWave(float timeDelta)
{
	if (goingUp == true)
	{
		if(angle < 2.0f)
		{
			//Ensure angle never goes above 2.0f
			if(angle + (waveFrequency) < 2.0f)
			{
				angle = angle + waveFrequency;
			}else
			{
				angle = 2.0f;
				goingUp = false;
			}
		}
	}

	if (goingUp == false)
	{
		if(angle > -2.0f)
		{
			//Ensure angle never goes below -2.0f
			if(angle - (waveFrequency) > -2.0f)
			{
				angle = angle - waveFrequency;
			}else
			{
				angle = -2.0f;
				goingUp = true;
			}
		}
	}
}
void SeaSurface::playSound()
{
	if(sound)
	{
		//sound->Stop();
		//sound->Reset();
		sound->Play();
	}
};

void SeaSurface::setObjectSound(CSound* _sound)
{
	sound = _sound;
};

void SeaSurface::draw(float timeDelta)
{
	this->playSound();

	//localDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	localDevice->SetMaterial(&material);
	localDevice->SetStreamSource(0, vb, 0, sizeof(Vertex));
	localDevice->SetIndices(ib);
	localDevice->SetFVF(FVF_VERTEX);
	texture.setTexture(timeDelta);
	localDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,0,numVertices,0,numTriangles); 

	//localDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

