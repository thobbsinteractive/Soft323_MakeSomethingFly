/* Name: viewObject.cpp
   Description: The methods for the viewObject class
*/
#include "camera.h"

Camera::Camera()
{
	cameraType = AIRCRAFT;

	pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

Camera::Camera(CameraType exCameraType)
{
	cameraType = AIRCRAFT;

	pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

Camera::~Camera()
{

}

void Camera::getPosition(D3DXVECTOR3* exPos)
{
	*exPos = pos;
}

void Camera::setPosition(D3DXVECTOR3* exPos)
{
	pos = *exPos;
}

void Camera::getRight(D3DXVECTOR3* exRight)
{
	*exRight = right;
}

void Camera::setRight(D3DXVECTOR3* exRight)
{
	right = *exRight;
}

void Camera::getUp(D3DXVECTOR3* exUp)
{
	*exUp = up;
}

void Camera::setUp(D3DXVECTOR3* exUp)
{
	up = *exUp;
}

void Camera::getLook(D3DXVECTOR3* exLook)
{
	*exLook = look;
}

void Camera::setLook(D3DXVECTOR3* exLook)
{
	look = *exLook;
}

void Camera::walk(float units)
{
	// move only on xz plane for land object
	if( cameraType == LANDOBJECT )
		pos += D3DXVECTOR3(look.x, 0.0f, look.z) * units;

	if( cameraType == AIRCRAFT )
		pos += look * units;
}

void Camera::strafe(float units)
{
	// move only on xz plane for land object
	if( cameraType == LANDOBJECT )
		pos += D3DXVECTOR3(right.x, 0.0f, right.z) * units;

	if( cameraType == AIRCRAFT )
		pos += right * units;
}

void Camera::fly(float units)
{
	// move only on y-axis for land object
	if( cameraType == LANDOBJECT )
		pos.y += units;

	if( cameraType == AIRCRAFT )
		pos += up * units;
}

void Camera::pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &right,	angle);

	// rotate up and look around right vector
	D3DXVec3TransformCoord(&up,&up, &T);
	D3DXVec3TransformCoord(&look,&look, &T);
}

void Camera::yaw(float angle)
{
	D3DXMATRIX T;

	// rotate around own up vector for aircraft
	D3DXMatrixRotationAxis(&T, &up, angle);

	// rotate right and look around up or y-axis
	D3DXVec3TransformCoord(&right,&right, &T);
	D3DXVec3TransformCoord(&look,&look, &T);
}

void Camera::roll(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &look,	angle);

	// rotate up and right around look vector
	D3DXVec3TransformCoord(&right,&right, &T);
	D3DXVec3TransformCoord(&up,&up, &T);
}

void Camera::getViewMatrix(D3DXMATRIX* V)
{
	// Keep camera's axes orthogonal to eachother
	D3DXVec3Normalize(&look, &look);

	D3DXVec3Cross(&up, &look, &right);
	D3DXVec3Normalize(&up, &up);

	D3DXVec3Cross(&right, &up, &look);
	D3DXVec3Normalize(&right, &right);

	// Build the view matrix:
	float x = -D3DXVec3Dot(&right, &pos);
	float y = -D3DXVec3Dot(&up, &pos);
	float z = -D3DXVec3Dot(&look, &pos);

	(*V)(0,0) = right.x; (*V)(0, 1) = up.x; (*V)(0, 2) = look.x; (*V)(0, 3) = 0.0f;
	(*V)(1,0) = right.y; (*V)(1, 1) = up.y; (*V)(1, 2) = look.y; (*V)(1, 3) = 0.0f;
	(*V)(2,0) = right.z; (*V)(2, 1) = up.z; (*V)(2, 2) = look.z; (*V)(2, 3) = 0.0f;
	(*V)(3,0) = x;       (*V)(3, 1) = y;    (*V)(3, 2) = z;      (*V)(3, 3) = 1.0f;
}

void Camera::setCameraType(CameraType exCameraType)
{
	cameraType = exCameraType;
}
