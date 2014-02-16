#ifndef _FBXMODEL_
#define _FBXMODEL_

#include <DirectXMath.h>


struct Polygon
{
	DirectX::XMFLOAT3 vertex1;
	DirectX::XMFLOAT3 vertex2;
	DirectX::XMFLOAT3 vertex3;
};

struct Model
{
	// Multiple lists of vertices (meshes)
		// Position
		// Normal
		// Color
		// Binormal
		// Tangent

	// Transform matrix
	// Position
	// Rotation
	// Scale

	// Material slots

	DirectX::XMFLOAT3* Vertices;
	unsigned int NumVertices;

	~Model()
	{
		delete [] Vertices;
	}

};

#endif