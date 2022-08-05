// Creates an animated texture


#include "animatedTexture.h"


animatedTexture::animatedTexture()
{
	currentFrame = 0;
	frameTime = 0.0f;
	frameSpeed = 0.0f;
}

animatedTexture::~animatedTexture()
{
	for(int i = 0; i < (int)Textures.size(); i++)
		d3d::Release<IDirect3DTexture9*>(Textures[i]);
}

void animatedTexture::setupTexture(IDirect3DDevice9* device,float _frameSpeed)
{
	frameSpeed = _frameSpeed;
	localDevice = device;
}

void animatedTexture::addTexture(char* textureName)
{
	IDirect3DTexture9* temp = 0;
	D3DXCreateTextureFromFileA(localDevice,textureName,&temp);
	Textures.push_back(temp);
}

void animatedTexture::setTexture(float timeDelta)
{
	if((int)Textures.size() > 0)
	{
		frameTime = frameTime + timeDelta;

		if(frameTime > frameSpeed)
		{
			frameTime = 0.0f;

			if(currentFrame < ((int)Textures.size()-1))
			{
				currentFrame = currentFrame++;
			}else
			{
				currentFrame = 0;
			}
		}
		localDevice->SetTexture(0,Textures[currentFrame]);
	}
}