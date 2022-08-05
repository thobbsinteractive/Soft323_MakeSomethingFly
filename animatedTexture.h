//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: animatedTexture.h
// Author: Tim Hobbs
// Description: creates an animated texture
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __animatedTextureH__
#define __animatedTextureH__

#include "d3dUtility.h"

using std::vector;

class animatedTexture
{
public:
	animatedTexture();
	~animatedTexture();
	void setupTexture(IDirect3DDevice9* device,float _frameSpeed);
	void addTexture(char* textureName);
	void setTexture(float timeDelta);
private:
	IDirect3DDevice9* localDevice;
	std::vector<IDirect3DTexture9*> Textures;
	float frameSpeed;
	float frameTime;
	int currentFrame;
};
#endif //__animatedTextureH__