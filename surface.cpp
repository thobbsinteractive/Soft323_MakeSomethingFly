//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: surface
// loosley based on the cube class by Frank Luna, this class draws a cube textured with the 
// textures passed to it during creation.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "surface.h"
#include "vertex.h"

Surface::Surface()
{

}

Surface::~Surface()
{
	if(vb){vb->Release(); vb = 0;}
	if(ib){ib->Release(); ib = 0;}
}

void Surface::setupSurface(IDirect3DDevice9* device,int _numVertsPerRow,
				 int _numVertsPerCol,
				 int _cellSpacing,float _textureU,float _textureV)
{
	// save a ptr to the device

	localDevice    = device;

	textureU = _textureU;
	textureV = _textureV;
	numVertsPerRow = _numVertsPerRow;
	numVertsPerCol = _numVertsPerCol;
	cellSpacing    = _cellSpacing;

	numCellsPerRow = numVertsPerRow - 1;
	numCellsPerCol = numVertsPerCol - 1;

	width = numCellsPerRow * cellSpacing;
	depth = numCellsPerCol * cellSpacing;

	numVertices  = numVertsPerRow * numVertsPerCol;
	numTriangles = numCellsPerRow * numCellsPerCol * 2;

	// Material Properties
	material.Specular.r = 1.0;
	material.Specular.g = 1.0;
	material.Specular.b = 1.0;
	material.Specular.a = 0;
	material.Power	  = 100;
	material.Diffuse.r = 1.0;
	material.Diffuse.g = 1.0;
	material.Diffuse.b = 1.0;
	material.Diffuse.a = 0;
	material.Ambient.r = 0.0;
	material.Ambient.g = 0.0;
	material.Ambient.b = 0.0;
	material.Ambient.a = 0.0;
	material.Emissive.r= 1.0;
	material.Emissive.g= 1.0;
	material.Emissive.b= 1.0;
	material.Emissive.a= 0;


	// compute the vertices
	if( !computeVertices() )
	{
		::MessageBox(0, L"computeVertices - FAILED", 0, 0);
		::PostQuitMessage(0);
	}

	// compute the indices
	if( !computeIndices() )
	{
		::MessageBox(0, L"computeIndices - FAILED", 0, 0);
		::PostQuitMessage(0);
	}
}

bool Surface::computeVertices()
{
	HRESULT hr = 0;

	hr = localDevice->CreateVertexBuffer(
		numVertices * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&vb,
		0);

	if(FAILED(hr))
		return false;

	// coordinates to start generating vertices at
	int startX = -width / 2;
	int startZ =  depth / 2;

	// coordinates to end generating vertices at
	int endX =  width / 2;
	int endZ = -depth / 2;

	// compute the increment size of the texture coordinates
	// from one vertex to the next.

	float uCoordIncrementSize = 1.0f / textureU;
	float vCoordIncrementSize = 1.0f / textureV;


	Vertex* v = 0;
	vb->Lock(0, 0, (void**)&v, 0);

	int i = 0;
	for(int z = startZ; z >= endZ; z -= cellSpacing)
	{
		int j = 0;
		for(int x = startX; x <= endX; x += cellSpacing)
		{
			// compute the correct index into the vertex buffer
			// based on where we are in the nested loop.
			int index = i * numVertsPerRow + j;

			v[index] = Vertex(
				(float)x,
				0.0f,
				(float)z,
				0.0f,
				0.0f,
				-1.0f,
				(float)j * uCoordIncrementSize,
				(float)i * vCoordIncrementSize);

			j++; // next column
		}
		i++; // next row
	}

	vb->Unlock();

	return true;
}

bool Surface::computeIndices()
{
	HRESULT hr = 0;

	hr = localDevice->CreateIndexBuffer(
		numTriangles * 3 * sizeof(WORD), // 3 indices per triangle
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&ib,
		0);

	if(FAILED(hr))
		return false;

	WORD* indices = 0;
	ib->Lock(0, 0, (void**)&indices, 0);

	// index to start of a group of 6 indices that describe the
	// two triangles that make up a quad
	int baseIndex = 0;

	// loop through and compute the triangles of each quad
	for(int i = 0; i < numCellsPerCol; i++)
	{
		for(int j = 0; j < numCellsPerRow; j++)
		{
			indices[baseIndex]     =   i   * numVertsPerRow + j;
			indices[baseIndex + 1] =   i   * numVertsPerRow + j + 1;
			indices[baseIndex + 2] = (i+1) * numVertsPerRow + j;

			indices[baseIndex + 3] = (i+1) * numVertsPerRow + j;
			indices[baseIndex + 4] =   i   * numVertsPerRow + j + 1;
			indices[baseIndex + 5] = (i+1) * numVertsPerRow + j + 1;

			// next quad
			baseIndex += 6;
		}
	}
	ib->Unlock();

	return true;
}

void Surface::draw()
{
	//localDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	localDevice->SetMaterial(&material);
	localDevice->SetStreamSource(0, vb, 0, sizeof(Vertex));
	localDevice->SetIndices(ib);
	localDevice->SetFVF(FVF_VERTEX);
	localDevice->SetTexture(0,texture);
	localDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,0,numVertices,0,numTriangles); 
	
	//localDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}