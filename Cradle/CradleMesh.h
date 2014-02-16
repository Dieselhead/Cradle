#ifndef _CRADLEMESH_
#define _CRADLEMESH_

#include "CradleResource.h"
#include "CradleVertexLayouts.h"


class CradleMesh : public CradleResource
{
public:
	CradleMesh(std::wstring _name , CradleFileManager* fm, CradleVertexDeclarations::VertexPosTex* verts, int numVerts /* */);
	CradleMesh(std::wstring _name, CradleFileManager* fm, CradleVertexDeclarations::VertexPosTex* uniqueVerts, int numVerts, DWORD* indices, int numIndices);
	virtual ~CradleMesh();

	ID3D11Buffer* GetVertexBuffer();
	int GetNumVertices();

	ID3D11Buffer* GetIndexBuffer() { return m_indexBuffer; }
	int GetNumIndices() { return m_numIndices; }

	void Draw(DirectX::XMFLOAT4X4* matrix);

public:
	
	CradleVertexDeclarations::VertexPosTex* m_vertices;
	int m_numVertices;
	DWORD* m_indices;
	int m_numIndices;

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

};

#endif