#include "deviceCapabilities.h"



DeviceCapabilities::DeviceCapabilities()
{

}
DeviceCapabilities::~DeviceCapabilities()

HRESULT hr = 0;

	// Acquire a pointer to an IDirect3D9 inteface called 'd3d9' to get 
	// graphics card capsabilities
	IDirect3D9 * d3d9;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	// Initialize D3DCAPS9 with actual Hardware Capabilities
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(
			D3DADAPTER_DEFAULT, // [in] Denotes primary display adapter.
			D3DDEVTYPE_HAL,		// [in] Specifies the device type
			&caps);				// Return out cap pointer with a filled D3DCAPS9 structure (called "caps")
								// that contains the capabilities of primary display adapter.
