//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: billboardSprite
// Draws a simple sprite to set dimensiones
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "billboardSprite.h"

billboardSprite::billboardSprite()
{

}

billboardSprite::~billboardSprite()
{
	if(_vb)
	{
		_vb->Release(); _vb = 0;
	};

	if(_ib)
	{
		_ib->Release(); _ib = 0;
	};
	//d3d::Release<IDirect3DTexture9>texture;
}

void billboardSprite::setupSprite(IDirect3DDevice9* device,float _sizeX, float _sizeY,char texturePath[200])
{
	// save a ptr to the device
	localDevice = device;

	sizeX = _sizeX;
	sizeY = _sizeY;

	material.Specular.r = 0;
	material.Specular.g = 0;
	material.Specular.b = 0;
	material.Specular.a = 0;
	material.Power	  = 100;
	material.Diffuse.r = 1.0;
	material.Diffuse.g = 1.0;
	material.Diffuse.b = 1.0;
	material.Diffuse.a = 0;
	material.Ambient.r = 1.0;
	material.Ambient.g = 1.0;
	material.Ambient.b = 1.0;
	material.Ambient.a = 0.0;
	material.Emissive.r= 1.0;
	material.Emissive.g= 1.0;
	material.Emissive.b= 1.0;
	material.Emissive.a= 0;

	localDevice->CreateVertexBuffer(
		24 * sizeof(Vertex), 
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&_vb,
		0);

	_vb->Lock(0, 0, (void**)&v, 0);

	// build plane
	
	v[0] = Vertex(-1.0f * (sizeX/2), -1.0f * (sizeY/2), 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[1] = Vertex( 1.0f * (sizeX/2), -1.0f * (sizeY/2), 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[2] = Vertex( 1.0f * (sizeX/2),  1.0f * (sizeY/2), 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[3] = Vertex(-1.0f * (sizeX/2),  1.0f * (sizeY/2), 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    ////////////// X /////////////// Y //////////////// Z //// nZ // nY // nZ // U /// V //
	_vb->Unlock();

	localDevice->CreateIndexBuffer(
		36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_ib,
		0);

	WORD* i = 0;
	_ib->Lock(0, 0, (void**)&i, 0);

	// fill in the front face index data
	i[0] = 2; i[1] = 1; i[2] = 0;
	i[3] = 3; i[4] = 2; i[5] = 0;

	_ib->Unlock();

	// save the loaded texture

	D3DXCreateTextureFromFileA(localDevice,texturePath,&texture);
}

void billboardSprite::draw()
{
	// use alpha from texture
	localDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	localDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	localDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	localDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    localDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	localDevice->SetMaterial(&material);

	localDevice->SetStreamSource(0, _vb, 0, sizeof(Vertex));
	localDevice->SetIndices(_ib);
	localDevice->SetFVF(FVF_VERTEX);
	localDevice->SetTexture(0,texture);
	localDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,0,6,0,3); 
	localDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}