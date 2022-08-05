#include "d3dUtility.h"

class Camera
{
public:
	enum CameraType { LANDOBJECT, AIRCRAFT};

	Camera();
	Camera(CameraType cameraType);
	~Camera();

	void strafe(float units); // left/right
	void fly(float units); // up/down
	void walk(float units); // forward/backward

	void pitch(float angle); // rotate on right vector
	void yaw(float angle); // rotate on up vector
	void roll(float angle); // rotate on look vector
	void getViewMatrix(D3DXMATRIX* V);
	void setCameraType(CameraType cameraType);
	void getPosition(D3DXVECTOR3* pos);
	void setPosition(D3DXVECTOR3* pos);
	void getRight(D3DXVECTOR3* _right);
	void setRight(D3DXVECTOR3* _right);
	void getUp(D3DXVECTOR3* _up);
	void setUp(D3DXVECTOR3* _up);
	void getLook(D3DXVECTOR3* _look);
	void setLook(D3DXVECTOR3* _Look);

private:
	CameraType	cameraType;
	D3DXVECTOR3	right;
	D3DXVECTOR3	up;
	D3DXVECTOR3	look;
	D3DXVECTOR3 pos;
};