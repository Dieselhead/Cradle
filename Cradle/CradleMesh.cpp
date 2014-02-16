#include "CradleMesh.h"
#include "CradleFileManager.h"

CradleMesh::CradleMesh(std::wstring _name, CradleFileManager* fm, CradleVertexDeclarations::VertexPosTex* verts, int numVerts /* */)
	: CradleResource(_name, fm)
{
	m_vertices = verts;
	m_numVertices = numVerts;

	m_indices = 0;

	wprintf_s(L"Creating Mesh with %d verts\n", numVerts);
	


	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(CradleVertexDeclarations::VertexPosTex) * m_numVertices;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(subData));
	subData.pSysMem = m_vertices;

	HRESULT hr = m_fileManager->GetCD3D()->m_d3dDevice->CreateBuffer(&bufferDesc, &subData, &m_vertexBuffer);
	HRPRINT(hr, L"Failed to create vertex buffer\n", L"Vertex buffer created\n");
	
	fm->AddMesh(this);
}

CradleMesh::CradleMesh(std::wstring _name, CradleFileManager* fm, CradleVertexDeclarations::VertexPosTex* uniqueVerts, int numVerts, DWORD* indices, int numIndices)
	: CradleResource(_name, fm)
{
	m_vertices = uniqueVerts;
	m_numVertices = numVerts;
	m_indices = indices;
	m_numIndices = numIndices;

	wprintf_s(L"Creating Mesh with %d verts\n", numVerts);



	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(CradleVertexDeclarations::VertexPosTex) * m_numVertices;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(subData));
	subData.pSysMem = m_vertices;

	HRESULT hr = m_fileManager->GetCD3D()->m_d3dDevice->CreateBuffer(&bufferDesc, &subData, &m_vertexBuffer);
	HRPRINT(hr, L"Failed to create vertex buffer\n", L"Vertex buffer created\n");





	D3D11_BUFFER_DESC indexDesc;
	ZeroMemory(&indexDesc, sizeof(indexDesc));
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.ByteWidth = sizeof(DWORD) * m_numIndices;
	indexDesc.CPUAccessFlags = 0;
	indexDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = m_indices;

	hr = m_fileManager->GetCD3D()->m_d3dDevice->CreateBuffer(&indexDesc, &indexData, &m_indexBuffer);
	HRPRINT(hr, L"Failed to create index buffer\n", L"Index buffer created\n");

	fm->AddMesh(this);
}

CradleMesh::~CradleMesh()
{
	if (m_vertices) delete[] m_vertices;
	m_vertices = nullptr;

	m_numVertices = 0;

	if (m_indices) delete[] m_indices;
	m_indices = nullptr;

	if (m_vertexBuffer) m_vertexBuffer->Release();
	m_vertexBuffer = nullptr;

	if (m_indexBuffer) m_indexBuffer->Release();
	m_indexBuffer = nullptr;
}

ID3D11Buffer* CradleMesh::GetVertexBuffer()
{
	return m_vertexBuffer;
}

int CradleMesh::GetNumVertices()
{
	return m_numVertices;
}


void CradleMesh::Draw(DirectX::XMFLOAT4X4* matrix)
{
	m_fileManager->DrawMesh(m_name, matrix);
}