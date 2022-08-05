#include "d3dUtility.h"
#include "viewObject.h"
#include "objectTransform.h"
#include "dragon.h"
#include "light.h"
#include "capabilities.h"
#include "d3dfont.h"
#include "cubeEnviroment.h"
#include "billboardSprite.h"
#include "pSystem.h"
#include "snow.h"
#include "seaSurface.h"

// Direct Input
#include "userInputs.h"
#include "gameInput.h"

// Sound Includes
#include "dxutil.h"
#include <dsound.h>
#include "dsutil.h" 

using std::vector;

//Globals
IDirect3DDevice9* device=0;
ViewObject	meshDragon,
			meshWingLeft,
			meshWingRight,
			meshTail,
			meshSplash;
//Sea
SeaSurface sea;

//Effects
ID3DXEffect* FogEffect   = 0;
D3DXHANDLE FogTechHandle = 0;
D3DXHANDLE SkyFogTechHandle = 0;
D3DXHANDLE ReflectTechHandle = 0;

 /* Allocate space for an enviromentCube */
cubeEnviroment TheEnviroment;

//Light
Light sun;

// Dragon translation
dragon Dragon(100.0f,D3DXVECTOR3(0.0f,200.0f,0.0f),D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.0f, 0.0f, 1.0f));

// Sprites
billboardSprite fireSprite;

Capabilities configWindow;
CD3DFont* Font = 0;

Snow* snowSprites;

//inputs
GameInput input;
UserInputs keyPressesInput;

//sounds
CSoundManager *soundManager = NULL;
CSound        *fireBallSound = NULL;
CSound        *waterSound = NULL;
CSound		  *wingSound = NULL;
CSound		  *splashSound = NULL;

// Global Variables
int width = 1024;
int height = 768;
DWORD FrameCnt;        // The number of frames that have occurred.
float TimeElapsed = 0; // The time that has elapsed so far
float FPS = 0;         // The frames rendered per second.
char FPSString[9];     // String to display current frames per second
char Values[200];      // String to display current positional information
bool enviromentOn = true;


bool LoadGeomemerty()
{
	bool loadOkay;

	//Load select mesh from path and Get a pointer to an ID3DXMesh in video memory containing the model data
	//And pointers to Vectors containing Material and Texture Data.
	loadOkay = meshDragon.loadMeshIntoBuffer("models/dragonMesh/dragonBody.x",device);
	loadOkay = meshWingLeft.loadMeshIntoBuffer("models/dragonMesh/dragonLeftWing.x",device);
	loadOkay = meshWingRight.loadMeshIntoBuffer("models/dragonMesh/dragonRightWing.x",device);
	loadOkay = meshTail.loadMeshIntoBuffer("models/dragonMesh/dragonTail.x",device);
	loadOkay = meshSplash.loadMeshIntoBuffer("models/splash/splash.x",device);

	return loadOkay;
}

void Setup_Snow()
{
	//
	// Create Snow System.
	//

	BoundingBox boundingBox;
	boundingBox.min = D3DXVECTOR3(-500.0f, -500.0f, -500.0f);
	boundingBox.max = D3DXVECTOR3( 500.0f, 500.0f,500.0f);
	snowSprites = new Snow(&boundingBox, 5000);
	snowSprites->init(device, "textures/sprites/snowflake.dds");
}

void Setup_Sound(HWND hwnd)
{

	// Nigels Code, Thanks

	soundManager = new CSoundManager();
	HRESULT res;
	

	if (soundManager == NULL)
	{
		MessageBox(NULL, L"Didn't initialise sound manager", L"MakeSomethingFly.exe", MB_OK);
	}

	res = soundManager->Initialize(hwnd, DSSCL_PRIORITY);
		
	if (FAILED(res))	
	{
		MessageBox(NULL, L"Can't initialise sound manager", L"MakeSomethingFly.exe", MB_OK);
	}

	res = soundManager->SetPrimaryBufferFormat(2,     //2 channels.
											   22050, //sampling rate.
											   16);	  //bits per sample.
	
	if (FAILED(res))
	{
		MessageBox(NULL, L"Can't set sound buffer format", L"MakeSomethingFly.exe", MB_OK);
	}
	
	if (FAILED(res = soundManager->Create(&fireBallSound, L"sounds/fireBall.wav", 0, GUID_NULL)))
	{
		MessageBox(NULL, L"Didn't initialise fireBallSound", L"MakeSomethingFly.exe", MB_OK);
	}else
	{
		Dragon.addObjectSound(fireBallSound);
	}

	if (FAILED(res = soundManager->Create(&wingSound, L"sounds/wingSound.wav", 0, GUID_NULL)))
	{
		MessageBox(NULL, L"Didn't initialise wingSound", L"MakeSomethingFly.exe", MB_OK);
	}

	if (FAILED(res = soundManager->Create(&waterSound, L"sounds/ocean.wav", 0, GUID_NULL)))
	{
		MessageBox(NULL, L"Didn't initialise water Sound", L"MakeSomethingFly.exe", MB_OK);
	}else
	{
		sea.setObjectSound(waterSound);
	}

	if (FAILED(res = soundManager->Create(&splashSound, L"sounds/splash.wav", 0, GUID_NULL)))
	{
		MessageBox(NULL, L"Didn't initialise splash Sound", L"MakeSomethingFly.exe", MB_OK);
	}else
	{
		Dragon.addObjectSound(splashSound);
	}
}



void Place_Scenery(float timeDelta,bool enviromentOn)
{	
	D3DXMATRIX sceneryScale,sceneryLocation;
	
	D3DXMatrixScaling(&sceneryScale,1.0f,1.0f,1.0f);
	D3DXMatrixTranslation(&sceneryLocation, 0.0f,0.0f,0.0f);
	
	D3DXMATRIX World = sceneryScale * sceneryLocation;
	
	device->SetTransform(D3DTS_WORLD, &World);
	sea.draw(timeDelta);
}

void Draw_SkyBox()
{	
	D3DXMATRIX sceneryScale,sceneryLocation;
	D3DXVECTOR3 pos;
	Dragon.getPosition(&pos);
	
	// Draw skybox
	D3DXMatrixScaling(&sceneryScale,10000.0f,5000.0f,10000.0f);
	D3DXMatrixTranslation(&sceneryLocation, pos.x,pos.y,pos.z);
	D3DXMATRIX World = sceneryScale * sceneryLocation;
	
	device->SetTransform(D3DTS_WORLD, &World);
	TheEnviroment.draw();

}

bool Setup()
{
	if (!LoadGeomemerty())
	{
		::MessageBox(0, L"LoadGeomemerty() - FAILED",0,0);
		return 0;
	}

	//
	// Create a font object and initialize it.
	//

	Font = new CD3DFont(L"Times New Roman", 16, 0);
	Font->InitDeviceObjects( device );
	Font->RestoreDeviceObjects();

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
void Draw_Dragon(float timeDelta)
{
	Dragon.update(timeDelta);

	D3DXMATRIX World;
	D3DXVECTOR4 lightPosition = D3DXVECTOR4(-10000,2000,-10000,0.0f);
	D3DXPLANE groundPlane = D3DXPLANE(0.0f,1.0f,0.0f,0.0f);

	// Draw All the Shadows First
	Dragon.getMatrix(&World);
	device->SetTransform(D3DTS_WORLD, &World);
	meshDragon.drawPlaneShadows(lightPosition,groundPlane,0.5f,World);

	//Draw all the shadows for the limbs
	for (int i=0; i < Dragon.getNumberofLimbs();i++)
	{
		Dragon.getLimbMatrix(i,&World,timeDelta);
		device->SetTransform(D3DTS_WORLD, &World);
		
		// Will have to replace with a mesh name string
	
		switch(i)
		{
			case 0: meshWingLeft.drawPlaneShadows(lightPosition,groundPlane,0.5f,World);
			break;
			case 1: meshWingRight.drawPlaneShadows(lightPosition,groundPlane,0.5f,World);
			break;
			case 2: meshTail.drawPlaneShadows(lightPosition,groundPlane,0.5f,World);
			break;
			default: meshWingLeft.drawObject();
		}
	}

	// Nows we have finished drawing the shadows we can draw the actual mesh

	Dragon.getMatrix(&World);
	device->SetTransform(D3DTS_WORLD, &World);
	meshDragon.drawObject();


	// Draw all the limbs
	for (int i=0; i < Dragon.getNumberofLimbs();i++)
	{
		Dragon.getLimbMatrix(i,&World,timeDelta);
		device->SetTransform(D3DTS_WORLD, &World);
		
		// Will have to replace with a mesh name string
	
		switch(i)
		{
			case 0: meshWingLeft.drawObject();
			break;
			case 1: meshWingRight.drawObject();
			break;
			case 2: meshTail.drawObject();
			break;
			default: meshWingLeft.drawObject();
		}
	}
}


void Draw_Dragon_Reflection(float timeDelta,D3DXMATRIX ReflectionMatrix)
{
	D3DXMATRIX World;
	D3DXMATRIX Translation;
	D3DXPLANE groundPlane = D3DXPLANE(0.0f,1.0f,0.0f,0.0f);

	Dragon.getMatrix(&Translation);
	World = Translation * ReflectionMatrix;
	device->SetTransform(D3DTS_WORLD, &World);
	meshDragon.drawObject();

	

	// Draw all the limbs
	for (int i=0; i < Dragon.getNumberofLimbs();i++)
	{
		Dragon.getLimbMatrix(i,&Translation,timeDelta);
		World = Translation * ReflectionMatrix;
		device->SetTransform(D3DTS_WORLD, &World);
		
		// Will have to replace with a mesh name string
	
		switch(i)
		{
			case 0: meshWingLeft.drawObject();
			break;
			case 1: meshWingRight.drawObject();
			break;
			case 2: meshTail.drawObject();
			break;
			default: meshWingLeft.drawObject();
		}
	}
}

void Draw_SkyBox_Reflection(D3DXMATRIX ReflectionMatrix)
{	
	D3DXMATRIX sceneryScale,sceneryLocation;
	D3DXVECTOR3 pos;
	Dragon.getPosition(&pos);
	
	// Draw skybox
	D3DXMatrixScaling(&sceneryScale,10000.0f,5000.0f,10000.0f);
	D3DXMatrixTranslation(&sceneryLocation, pos.x,pos.y,pos.z);
	D3DXMATRIX World = sceneryScale * sceneryLocation;
	World = World * ReflectionMatrix;
	
	device->SetTransform(D3DTS_WORLD, &World);
	TheEnviroment.draw();

}

void Draw_Sprite_Reflection(D3DXMATRIX ReflectionMatrix)
{
	D3DXVECTOR3 dragonPosition;
	D3DXMATRIX World;
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);

	for(int i =0; i < Dragon.getNumberofFireBalls(); i++)
	{
		Dragon.getFireMatrix(i,&World);
		World = World * ReflectionMatrix;
		device->SetTransform(D3DTS_WORLD, &World);
		fireSprite.draw();
	}
}

void Draw_Splashes_Reflection(D3DXMATRIX ReflectionMatrix)
{
	D3DXMATRIX World;
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);

	// Disable lighting
	device->SetRenderState( D3DRS_LIGHTING, FALSE );

	for(int i =0; i < Dragon.getNumberofSplashes(); i++)
	{
		Dragon.getSplashMatrix(i,&World);
		World = World * ReflectionMatrix;
		device->SetTransform(D3DTS_WORLD, &World);
		meshSplash.drawObject();
	}

	device->SetRenderState( D3DRS_LIGHTING, TRUE );
}

void Draw_Reflections(float timeDelta)
{
	// Half of this is Luna Code, half of this my code (specifically where rendering objects)

	//
	// Draw water mesh to stencil buffer ONLY.  In this way
	// only the stencil bits that correspond to the mirror will
	// be on.  Therefore, the reflected objects can only be rendered
	// where the stencil bits are turned on, and thus on the water 
	// only.
	//

	D3DXVECTOR3 pos;
	Dragon.getPosition(&pos);

    device->SetRenderState(D3DRS_STENCILENABLE,    true);
    device->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_ALWAYS);
    device->SetRenderState(D3DRS_STENCILREF,       0x1);
    device->SetRenderState(D3DRS_STENCILMASK,      0xffffffff);
    device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
    device->SetRenderState(D3DRS_STENCILZFAIL,     D3DSTENCILOP_KEEP);
    device->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);
    device->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_REPLACE);

	// disable writes to the depth and back buffers
    device->SetRenderState(D3DRS_ZWRITEENABLE, false);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ZERO);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

    // draw the mirror to the stencil buffer
	Place_Scenery(timeDelta,true);
	
	// re-enable depth writes
	device->SetRenderState( D3DRS_ZWRITEENABLE, true );

	// only draw reflected objects to the pixels where the mirror
	// was drawn to.
	device->SetRenderState(D3DRS_STENCILFUNC,  D3DCMP_EQUAL);
    device->SetRenderState(D3DRS_STENCILPASS,  D3DSTENCILOP_KEEP);

	// position reflection
	D3DXMATRIX WorldMatrix, DragonMatrix ,ReflectionMatrix;
	D3DXPLANE groundPlane = D3DXPLANE(0.0f,1.0f,0.0f,0.0f);

	D3DXMatrixReflect(&ReflectionMatrix, &groundPlane);

	// clear depth buffer and blend the reflected objects with the mirror
	device->SetRenderState(D3DRS_ZENABLE, false);
	device->SetRenderState(D3DRS_ZWRITEENABLE, false );
	//device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	//device->SetRenderState(D3DRS_ZENABLE, false);

	device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_DESTCOLOR);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);


	// Finally, draw the reflected objects
	Draw_SkyBox_Reflection(ReflectionMatrix);
	Draw_Dragon_Reflection(timeDelta,ReflectionMatrix);
	Draw_Splashes_Reflection(ReflectionMatrix);
	Draw_Sprite_Reflection(ReflectionMatrix);

	
	// Restore render states.
	device->SetRenderState(D3DRS_ZENABLE, true);
	device->SetRenderState(D3DRS_ZWRITEENABLE, true );

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	device->SetRenderState(D3DRS_STENCILENABLE, false);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void Update_Dragon(float timeDelta)
{
	//
	// Update the inputs to check for changes
	//
	input.checkInputs();
	

	
	if(keyPressesInput.fireKey == true)
	{
		Dragon.shoot();
	}


	if(keyPressesInput.forwardKey == true)
	{
		Dragon.walk(8.0f * timeDelta,true);
	}

	if(keyPressesInput.backKey == true)
	{
		Dragon.walk(-8.0f * timeDelta,true);
	}

	if(keyPressesInput.upKey == true)
	{
		Dragon.pitch(2.0f * timeDelta);
	}

	if(keyPressesInput.downKey == true)
	{
		Dragon.pitch(-2.0f * timeDelta);
	}

	if(keyPressesInput.leftKey == true)
	{
		Dragon.yaw(-2.0f * timeDelta);
	}
			
	if(keyPressesInput.rightKey == true)
	{
		Dragon.yaw(2.0f * timeDelta);
	}

	/*
	if( ::GetAsyncKeyState('N') & 0x8000f )
	{
		Dragon.roll(2.0f * timeDelta);
	}

	if( ::GetAsyncKeyState('M') & 0x8000f )
	{
		Dragon.roll(-2.0f * timeDelta);
	}
	*/
}

void Setup_TextureFilters()
{
	//
	// Set texture filters.
	//

	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
}

void Setup_AlphaBlending()
{
	//
	// Set alpha blending states.
	//

	// use alpha in material's diffuse component for alpha
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	// use alpha channel in texture for alpha
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void Setup_Lights()
{
	// 
	// Set Lights.
	//
	
	D3DXVECTOR3 shipPos;
	Dragon.getPosition(&shipPos);

	D3DXVECTOR3 pos(-10000,2000,-10000);
	D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);

	D3DLIGHT9 light = sun.InitPointLight(&pos, &col);

	device->SetLight(0, &light);
	device->LightEnable(0, true);
	device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	device->SetRenderState(D3DRS_SPECULARENABLE, true);
}

void Projection_Matrix(float angle)
{

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		& proj,
		D3DX_PI * 0.25f,
		(float)width / (float)height,
		1.0f,
		100000.0f);

	device->SetTransform(D3DTS_PROJECTION, &proj);
}

void Render_Setup()
{
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);     //Default fillmode
	device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD); //Default shademode
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);        //Turn culling on
}

void Draw_Sprites()
{
	D3DXVECTOR3 dragonPosition;
	D3DXMATRIX World;
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);

	for(int i =0; i < Dragon.getNumberofFireBalls(); i++)
	{
		Dragon.getFireMatrix(i,&World);
		device->SetTransform(D3DTS_WORLD, &World);
		fireSprite.draw();
	}

	Dragon.getPosition(&dragonPosition);
	
	D3DXMatrixTranslation(&World,dragonPosition.x,dragonPosition.y,dragonPosition.z);
	snowSprites->updateBoundingBoxPosition(dragonPosition.x,dragonPosition.y,dragonPosition.z);
	device->SetTransform(D3DTS_WORLD, &I);
	snowSprites->render();
}

void Update_Camera(float timeDelta)
{
	D3DXMATRIX V;

	D3DXVECTOR3 cameraPosition;
	D3DXVECTOR3 cameraRotation;
	D3DXVECTOR3 cameraLookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXVECTOR3 look;
	D3DXVECTOR3 newLook;
	D3DXMATRIX RotationMatrix;
	D3DXVECTOR3 normedVector;

	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	Dragon.getPosition(&cameraPosition);
	Dragon.getPosition(&cameraLookAt);
	Dragon.getRotation(&cameraRotation);

	// Display the position  and speed of the Dragon
	sprintf(Values, "Pos:%f,%f,%f,\nLook:%f,%f,%f,\nRotation:%f,%f,%f,\nSpeedSet:%f\nCurrentSpeed:%f",
		cameraPosition.x,cameraPosition.y,cameraPosition.z,
		cameraLookAt.x,cameraLookAt.y,cameraLookAt.z,
		cameraRotation.x,cameraRotation.y,cameraRotation.z,
		Dragon.getCurrentSpeedSetting(),Dragon.getCurrentSpeed());
	Values[199] = '\0'; // mark end of string

	// Calculate Camera look at coordinates maybe move this into dragon class
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix,cameraRotation.x,cameraRotation.y,cameraRotation.z);
    D3DXVec3TransformCoord(&newLook, &look, &RotationMatrix);
    D3DXVec3Normalize(&normedVector, &newLook );

	cameraLookAt += normedVector * 100;
	cameraPosition -= normedVector * 200;

	D3DXVECTOR3 up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	
	cameraPosition.y = cameraPosition.y + 100;

	D3DXMatrixLookAtLH(&V, &cameraPosition,&cameraLookAt,&up);
	device->SetTransform(D3DTS_VIEW, &V);
}

//Draw water splashes
void Draw_Splashes()
{
	D3DXMATRIX World;
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);

	// Disable lighting
	device->SetRenderState( D3DRS_LIGHTING, FALSE );

	for(int i =0; i < Dragon.getNumberofSplashes(); i++)
	{
		Dragon.getSplashMatrix(i,&World);
		device->SetTransform(D3DTS_WORLD, &World);
		meshSplash.drawObject();
	}

	device->SetRenderState( D3DRS_LIGHTING, TRUE );
}

void Setup_Effects()
{
	//
	// Create effect.
	//
	HRESULT hr = 0;

	ID3DXBuffer* errorBuffer = 0;
	hr = D3DXCreateEffectFromFile(
		device,
		L"fog.txt",
		0,                // no preprocessor definitions
		0,                // no ID3DXInclude interface
		D3DXSHADER_DEBUG, // compile flags
		0,                // don't share parameters
		&FogEffect,
		&errorBuffer);

	// output any error messages
	if( errorBuffer )
	{
		::MessageBoxA(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
		d3d::Release<ID3DXBuffer*>(errorBuffer);
	}

	if(FAILED(hr))
	{
		::MessageBox(0, L"D3DXCreateEffectFromFile() - FAILED", 0, 0);
	}

	// 
	// Save Frequently Accessed Parameter Handles
	//

	FogTechHandle = FogEffect->GetTechniqueByName("Fog");
	SkyFogTechHandle = FogEffect->GetTechniqueByName("FogSkyBox");
	ReflectTechHandle = FogEffect->GetTechniqueByName("FogReflections");
}

void Setup_Camera()
{
	Update_Camera(0.0f);
	//TheCamera.placeCameraInScene(device);
}

void Setup_Dragon()
{
	// Wing1

	float speed = 0.2f;
	float movementRotationUpper = 0.2f;
	float movementRotationLower = -0.5f;
	char rotationAxis = 'r';
	bool rotateUp = false; //Direction

	Dragon.addLimb(100.0f,speed,movementRotationUpper,movementRotationLower,rotateUp,rotationAxis,wingSound);

	// Wing2
	//Could maybe mirror the matrix rather than go to all this bother.

	speed = 0.2f;
	movementRotationUpper = 0.5f;
	movementRotationLower = -0.2f;
	rotateUp = true; //Direction
	rotationAxis = 'r';

	Dragon.addLimb(100.0f,speed,movementRotationUpper,movementRotationLower,rotateUp,rotationAxis,wingSound);

	// Tail

	speed = 0.01f;
	movementRotationUpper = 0.2f;
	movementRotationLower = 0.0f;
	rotationAxis = 'p';

	Dragon.addLimb(100.0f,speed,movementRotationUpper,movementRotationLower,rotateUp,rotationAxis);
}

void CalcFPS(float timeDelta)
{
	//
	// Update: Compute the frames per second.
	//
	FrameCnt++;
	TimeElapsed += timeDelta;

	if(TimeElapsed >= 1.0f)
	{
		FPS = (float)FrameCnt / TimeElapsed;

		sprintf(FPSString, "%f", FPS);
		FPSString[8] = '\0'; // mark end of string

		TimeElapsed = 0.0f;
		FrameCnt = 0;
	}
}


/////////////////////////////////////////////////////////////////////////////////////
bool Display(float timeDelta)
{
/* The Display method calls the IDirect3Ddevice9::Clear method
   ,which clears the backbuffer and depth/stencil buffer to the
   colour black. We also implement here any and all drawing code.
*/

	if(device)
	{

		// Last Update Camera Position
		Update_Camera(timeDelta);

		CalcFPS(timeDelta);

		// UpdateSnow
		snowSprites->update(timeDelta);

		// UpdateWaves
		sea.wave(timeDelta);

		// Move dragon
		Update_Dragon(timeDelta);
	
		//
		// Render
		//

		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xffffffff, 1.0f, 0L);
		device->BeginScene();

		// Set Fog effect
		FogEffect->SetTechnique( SkyFogTechHandle );

		// Get number of passes in fog
		UINT numPasses = 0;
    	FogEffect->Begin(&numPasses, 0);

		D3DXMATRIX I;
		D3DXMatrixIdentity(&I);

		for(int i = 0; i < (int)numPasses; i++)
		{
			FogEffect->BeginPass(i);

			// Geometery Elements fogged go here

			// Draw Cubed Enviroment
			Draw_SkyBox();

			FogEffect->EndPass();
		}
		FogEffect->End();
		
	
		// Set Fog effect
		FogEffect->SetTechnique( FogTechHandle );

		// Get number of passes in fog
		numPasses = 0;
    	FogEffect->Begin(&numPasses, 0);

		for(int i = 0; i < (int)numPasses; i++)
		{
			FogEffect->BeginPass(i);

			// Geometery Elements fogged go here
			
			// Place and draw Enviroment
			Place_Scenery(timeDelta,enviromentOn);

			// Place and draw dragon
			Draw_Dragon(timeDelta);

			FogEffect->EndPass();
		}
		FogEffect->End();

		// Set Fog effect for reflections
		FogEffect->SetTechnique(ReflectTechHandle);

		// Get number of passes in fog
		numPasses = 0;
    	FogEffect->Begin(&numPasses, 0);
		D3DXMatrixIdentity(&I);

		for(int i = 0; i < (int)numPasses; i++)
		{
			FogEffect->BeginPass(i);

			// Draw reflected Enviroment
			Draw_Reflections(timeDelta);
			
			FogEffect->EndPass();
		}
		FogEffect->End();

		//Alpha blended Sprites and models have to be drawn after reflections
		Draw_Splashes();
		Draw_Sprites();

		//Draw FPS
		if( Font )
		{
			Font->DrawText(20, 20, 0xff000000, FPSString);
			Font->DrawText(20, 50, 0xff000000, Values);
		};
		
		device->EndScene();
		device->Present(0, 0, 0, 0);
	}
	return true;
}

void Cleanup()
{
	meshDragon.cleanUP();
	meshWingLeft.cleanUP();
	meshWingRight.cleanUP();
	meshTail.cleanUP();
	meshSplash.cleanUP();

	if( Font )
	{
		Font->InvalidateDeviceObjects();
		Font->DeleteDeviceObjects();
		d3d::Delete<CD3DFont*>(Font);
	}

	delete snowSprites;
}


//////////////// Main Application //////////////////

int WINAPI WinMain(HINSTANCE hinstance, 
				   HINSTANCE prevInstance, 
				   PSTR cmdLine, 
				   int showCmd)
{
	HWND hwnd = 0;

	int tempWidth = width;

	// Start window
	if (!d3d::InitD3D(hinstance,&hwnd,width,height))
	{
		::MessageBox(0, L"InitD3D() - FAILED",0,0);
		return 0;
	}

	// Get card config and start device
	if (!configWindow.getCapabilities(&width, &height,&hwnd,&device))
	{
		::MessageBox(0, L"Configuration failed",0,0);
		return 0;
	}

	if (tempWidth != width)
	{
		// Resize window
		d3d::InitD3D(hinstance,&hwnd,width,height);
	}

	//Setup Scene Meshes
	if (!Setup())
	{
		::MessageBox(0, L"Setup() - FAILED",0,0);
		return 0;
	}
	
	Setup_TextureFilters();

	Setup_AlphaBlending();

	Setup_Camera();

	Setup_Lights();

	Setup_Snow();

	// setup direct input
	input.setup_Input(&hinstance,&hwnd,&keyPressesInput);

	Projection_Matrix(90);

	Setup_Sound(hwnd);

	Render_Setup();

	Setup_Dragon();

	// skybox

	TheEnviroment.loadTextures(device);

	// sprites

	fireSprite.setupSprite(device,100,100,"textures/sprites/fireBall.dds");

	// Setup Sea
	sea.setupSurface(device,110,110,400,4.0f,4.0f);
	sea.setupAnimatedTextures();

	// Setup Effects
	Setup_Effects();

	d3d::EnterMsgLoop(Display);

	// If use has quit Clean up memory
	Cleanup();
	device->Release();

	return 0;
}