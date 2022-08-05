#ifndef __cubeVertexH__
#define __cubeVertexH__

struct CubeVertex
{
	CubeVertex(){}
	CubeVertex(
		float x, float y, float z, // vertix location
		float nx, float ny, float nz, //vertix normals
		float u, float v, float w) // texture coordinates
	{
		_x  = x;  _y  = y;  _z  = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u  = u;  _v  = v; _w = w;
	}
    float _x, _y, _z;
    float _nx, _ny, _nz;
    float _u, _v, _w; // texture coordinates
};
#define FVF_VERTEX_CUBE (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0))
//#define FVF_VERTEX_CUBE (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2)

#endif // __cubeVertexH__