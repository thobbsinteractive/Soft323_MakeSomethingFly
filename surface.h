//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: surface.h
// Author: Frank Luna
// Description: This creates a surface with as much detail as the programmer wants
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __surfaceH__
#define __surfaceH__

#include "d3dUtility.h"
#include <strsafe.h>
#include "vertex.h"

using std::vector;

class Surface
{
public:
	Surface();
	~Surface();
	void setupSurface(IDirect3DDevice9* device,
						int _numVertsPerRow,
						int _numVertsPerCol,
						int _cellSpacing,
						float _textureU,
						float _textureV);

	bool computeVertices();
	bool computeIndices();
	void draw();

	IDirect3DDevice9* localDevice;
	IDirect3DTexture9* texture;
	D3DMATERIAL9 material;
	IDirect3DVertexBuffer9* vb;
	IDirect3DIndexBuffer9*  ib;
	Vertex* v;

	int numVertsPerRow;
	int numVertsPerCol;
	int cellSpacing;

	int numCellsPerRow;
	int numCellsPerCol;
	int width;
	int depth;
	int numVertices;
	int numTriangles;

	float textureU;
	float textureV;

};
#endif //__surfaceH__