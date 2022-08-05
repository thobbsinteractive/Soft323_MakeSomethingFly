//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: cubeEnviroment
// loosley based on the cube class by Frank Luna, this class draws a cube textured with the 
// textures passed to it during creation.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "cubeEnviroment.h"
#include "vertex.h"

cubeEnviroment::cubeEnviroment()
{

}

cubeEnviroment::~cubeEnviroment()
{
	if(_vb){_vb->Release(); _vb = 0;}
	if(_ib){_ib->Release(); _ib = 0;}

	for(int i = 0; i < (int)Textures.size(); i++)
		d3d::Release<IDirect3DTexture9*>(Textures[i]);
}

void cubeEnviroment::loadTextures(IDirect3DDevice9* device)
{
	// save a ptr to the device
	localDevice = device;

	
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

	// build box

	// fill in the front face vertex data
	v[0] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	v[1] = Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[2] = Vertex( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[3] = Vertex( 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);

	// fill in the back face vertex data
	
	v[4] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[5] = Vertex( 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[6] = Vertex( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[7] = Vertex(-1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	
	// fill in the top face vertex data
	
	v[8]  = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	v[9]  = Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[10] = Vertex( 1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[11] = Vertex( 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

	// fill in the bottom face vertex data
	v[12] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	v[13] = Vertex( 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	v[14] = Vertex( 1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	v[15] = Vertex(-1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	// fill in the left face vertex data
	v[16] = Vertex(-1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[17] = Vertex(-1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[18] = Vertex(-1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[19] = Vertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	// fill in the right face vertex data
	v[20] = Vertex( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[21] = Vertex( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[22] = Vertex( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[23] = Vertex( 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
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

	// fill in the back face index data
	i[6] = 6; i[7]  = 5; i[8]  = 4;
	i[9] = 7; i[10] = 6; i[11] = 4;

	// fill in the top face index data
	i[12] = 10; i[13] =  9; i[14] = 8;
	i[15] = 11; i[16] = 10; i[17] = 8;

	// fill in the bottom face index data
	i[18] = 14; i[19] = 13; i[20] = 12;
	i[21] = 15; i[22] = 14; i[23] = 12;

	// fill in the left face index data
	i[24] = 18; i[25] = 17; i[26] = 16;
	i[27] = 19; i[28] = 18; i[29] = 16;

	// fill in the right face index data
	i[30] = 22; i[31] = 21; i[32] = 20;
	i[33] = 23; i[34] = 22; i[35] = 20;

	_ib->Unlock();

	// save the loaded texture

	IDirect3DTexture9* top = 0;
	IDirect3DTexture9* bottom = 0;
	IDirect3DTexture9* back = 0;
	IDirect3DTexture9* front = 0;
	IDirect3DTexture9* right = 0;
	IDirect3DTexture9* left = 0;

	D3DXCreateTextureFromFileA(localDevice,"textures/skybox/skyboxFront.jpg",&front);
	D3DXCreateTextureFromFileA(localDevice,"textures/skybox/skyboxBack.jpg",&back);
	D3DXCreateTextureFromFileA(localDevice,"textures/skybox/skyboxLeft.jpg",&left);
	D3DXCreateTextureFromFileA(localDevice,"textures/skybox/skyboxRight.jpg",&right);
	D3DXCreateTextureFromFileA(localDevice,"textures/skybox/skyboxTop.jpg",&top);
	D3DXCreateTextureFromFileA(localDevice,"textures/skybox/skyboxBottom.jpg",&bottom);
	
	Textures.push_back(front);
	Textures.push_back(back);
	Textures.push_back(top);
	Textures.push_back(bottom);
	Textures.push_back(left);
	Textures.push_back(right);
}

void cubeEnviroment::draw()
{
	// Clamp texture co-ordinates to get rid of horrible seam
	localDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU,  D3DTADDRESS_CLAMP );
	localDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV,  D3DTADDRESS_CLAMP );


	int j = 0;
	localDevice->SetMaterial(&material);

	localDevice->SetStreamSource(0, _vb, 0, sizeof(Vertex));
	localDevice->SetIndices(_ib);
	localDevice->SetFVF(FVF_VERTEX);
	
	for(int i = 0; i < (int)Textures.size(); i++)
	{
		j = i+1;
		localDevice->SetTexture(0,Textures[i]);
		localDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,0,6,6*i,2); 
	}
	
	// Reset texture coordinates
	localDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU,  D3DTADDRESS_WRAP );
	localDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV,  D3DTADDRESS_WRAP );
}