/* Name: viewObject.cpp
   Description: The methods for the viewObject class
   This class loads a requested mesh from a file into the vertex buffer.
   The mesh is then optimized and when requested drawn. 
   
   This class is seperate from the object transform class because you may 
   have 20 ojects with the same mesh and in doing so, you do not want to 
   needlessly fill the vertex buffer with the same mesh data.
*/
#include "viewObject.h"

ViewObject::ViewObject()
{
	adjBuffer = 0;
}

ViewObject::~ViewObject()
{

}

bool ViewObject::loadMeshIntoBuffer(char sysPath[],
									IDirect3DDevice9* Device)
{
	// save a ptr to the device
	localDevice = Device;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD numMtrls = 0;
	
	bool result = true;				// Initialise return result

	//MessageBox(NULL, sysPath, "Meshes.exe", MB_OK);

    // D3DXLoadMeshFromX
    // Load the mesh from the specified file
	if( FAILED( D3DXLoadMeshFromXA( sysPath, 
									D3DXMESH_MANAGED, 
                                    localDevice,
									&adjBuffer,
                                    &mtrlBuffer, 
									NULL, 
									&numMtrls, 
                                    &pMesh ) ) )
    {
		MessageBox(NULL, L"Could not find Mesh", L"Meshes.exe", MB_OK);
        result = false;
    }

	if (result)
	{
	//
	// Extract the materials, and load textures.
	//

	if( mtrlBuffer != 0 && numMtrls != 0 )
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for(int i = 0; i < (int)numMtrls; i++)
		{
			// the MatD3D property doesn't have an ambient value set
			// when its loaded, so set it now:
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// save the ith material
			Mtrls.push_back( mtrls[i].MatD3D );

			// check if the ith material has an associative texture
			if( mtrls[i].pTextureFilename != 0 )
			{
				// yes, load the texture for the ith subset
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFileA(
					localDevice,
					mtrls[i].pTextureFilename,
					&tex);

				// save the loaded texture
				Textures.push_back( tex );
			}
			else
			{
				// no texture for the ith subset
				Textures.push_back( 0 );
			}
		}
	}
	d3d::Release<ID3DXBuffer*>(mtrlBuffer); // done w/ buffer
	}
	
	if(pMesh)
	{
		optmizeMesh(); 
	}
    return result;
};

void ViewObject::optmizeMesh()
{
	//
	// Optimize the mesh.
	//
	HRESULT hr = NULL;

	hr = pMesh->OptimizeInplace(		
	D3DXMESHOPT_ATTRSORT |
	D3DXMESHOPT_COMPACT  |
	D3DXMESHOPT_VERTEXCACHE,
	(DWORD*)adjBuffer->GetBufferPointer(),
	0, 0, 0);

	if(FAILED(hr))
	{
		::MessageBox(0, L"OptimizeInplace() - FAILED", 0, 0);
	}
};

void ViewObject::drawObject()
{
	// use alpha from texture
	localDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	localDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	localDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	localDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    localDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for(int i = 0; i < (int)Mtrls.size(); i++)
	{
		localDevice->SetMaterial(&Mtrls[i]);
		localDevice->SetTexture(0,Textures[i]);
		pMesh->DrawSubset(i);
	}
	//localDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
};

void ViewObject::drawPlaneShadows(D3DXVECTOR4 vecLight, //Light location
								  D3DXPLANE floorPlane, // floorPlane
								  float transparency, // Transparency
								  D3DXMATRIX meshLocation) // This meshs location, we disgard it after use
{
	D3DMATERIAL9 shadowMaterial;
	D3DXMATRIX   matShadow, matShadowTransformed;


	//Set up stencile buffer to draw shadows only once per pixel.
	localDevice->SetRenderState(D3DRS_STENCILENABLE,    true);
    localDevice->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_EQUAL);
    localDevice->SetRenderState(D3DRS_STENCILREF,       0x0);
    localDevice->SetRenderState(D3DRS_STENCILMASK,      0xffffffff);
    localDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
    localDevice->SetRenderState(D3DRS_STENCILZFAIL,     D3DSTENCILOP_KEEP);
    localDevice->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);
    localDevice->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_INCR); // increment to 1


	D3DXVec4Normalize(&vecLight, &vecLight);  //Belt and baces.

	shadowMaterial.Specular.r = 0.0f;
	shadowMaterial.Specular.g = 0.0f;
	shadowMaterial.Specular.b = 0.0f;
	shadowMaterial.Specular.a = 0.0f;
	shadowMaterial.Power	  = 80.0f;
	shadowMaterial.Diffuse.r = 0.0f;
	shadowMaterial.Diffuse.g = 0.0f;
	shadowMaterial.Diffuse.b = 0.0f;
	shadowMaterial.Diffuse.a = transparency; // Transparency
	shadowMaterial.Ambient.r = 0.2f;
	shadowMaterial.Ambient.g = 0.2f;
	shadowMaterial.Ambient.b = 0.2f;
	shadowMaterial.Ambient.a = 0.8f;
	shadowMaterial.Emissive.r= 0.0f;
	shadowMaterial.Emissive.g= 0.0f;
	shadowMaterial.Emissive.b= 0.0f;
	shadowMaterial.Emissive.a= 0.0f;

	D3DXMatrixShadow(&matShadow, &vecLight, &floorPlane);	
	matShadowTransformed =  meshLocation * matShadow;

	// Place Shadow realtive to light
	localDevice->SetTransform(D3DTS_WORLD, &matShadowTransformed);

	// Disable depth buffer so that z-fighting doesn't occur when we
	// render the shadow on top of the floor. Shadow Must be draw first! Before mesh
	localDevice->SetRenderState(D3DRS_ZENABLE, false);

	// Draw Mesh with shadow material Is more efficent if outside the loop as the shadows
	// material does not change.
	localDevice->SetTexture(0,NULL);
	localDevice->SetMaterial(&shadowMaterial);
	localDevice->SetTexture(0,NULL);

	for(int i = 0; i < (int)Mtrls.size(); i++)
	{

		localDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		pMesh->DrawSubset(i);
		localDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}

	// Re-enable Z buffering
	localDevice->SetRenderState(D3DRS_ZENABLE, true);
	
	// Disable and reset stencil buffer.
	localDevice->SetRenderState(D3DRS_STENCILENABLE,    false);
};

void ViewObject::cleanUP()
{
	d3d::Release<ID3DXMesh*>(pMesh);
	d3d::Release<ID3DXBuffer*>(adjBuffer); // done w/ buffer

	for(int i = 0; i < (int)Textures.size(); i++)
		d3d::Release<IDirect3DTexture9*>( Textures[i] );
};