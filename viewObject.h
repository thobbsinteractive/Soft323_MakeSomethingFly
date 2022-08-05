//////////////////////////////////////////////////////////////////////////////////////////////////
// viewObject.h
// Author: Tim Hobbs /some Frank Luna code
// Description: Holds all mesh data, for drawing the mesh and drawing its shadows.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"
#include <strsafe.h>

using std::vector;

class ViewObject
{
public:
	ViewObject();
	~ViewObject();

	bool loadMeshIntoBuffer(char sysPath[], IDirect3DDevice9* localDevice);
	void drawObject();
	void drawPlaneShadows(D3DXVECTOR4 vecLight, //Light location
						  D3DXPLANE floorPlane, // floorPlane
						  float transparency, // Transparency
						  D3DXMATRIX meshLocation); // This meshs location
	void optmizeMesh();
	void cleanUP();

    template<class T> void Release(T t)
	{
		if(t)
		{
			t->Release();
			t = 0;
		}
	}

	template<class T> void Delete(T t)
	{
		if(t)
		{
			delete t;
			t = 0;
		}
	}

private:
	IDirect3DDevice9* localDevice;
	ID3DXMesh* pMesh;
	std::vector<D3DMATERIAL9> Mtrls;
	std::vector<IDirect3DTexture9*> Textures;
	ID3DXBuffer* adjBuffer;

};