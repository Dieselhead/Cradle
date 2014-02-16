#include "CradleFileManager.h"
#include "CradleD3D.h"
#include "Macros.h"
#include "CradleResource.h"
#include "ObjModel.h"
#include "CradleMesh.h"
#include "CradleVertexLayouts.h"

CradleFileManager::CradleFileManager(CradleD3D* cd3d)
{
	m_cd3d = cd3d;
}

CradleFileManager::~CradleFileManager()
{
	for (const auto& item : m_vertexShaders)
	{
		wprintf_s(L"First: %s\n", item.first);
		
		if (item.second)
		{
			if (item.second->VS)
			{
				item.second->VS->Release();
			}

			if (item.second->IL)
			{
				item.second->IL->Release();
			}

			delete item.second;
		}

	}

	for (const auto& item : m_pixelShaders)
	{
		wprintf_s(L"First: %s\n", item.first);

		if (item.second)
			item.second->Release();

	}

	for (const auto& mesh : m_meshes)
	{
		if (mesh.second)
			delete mesh.second;
	}
}

ID3D11PixelShader* CradleFileManager::GetPixelShader(wchar_t* token)
{
	if (m_pixelShaders.find(token) != m_pixelShaders.end())
		return m_pixelShaders[token];

	return nullptr;
}

ID3D11VertexShader* CradleFileManager::GetVertexShader(wchar_t* token)
{
	if (m_vertexShaders.find(token) != m_vertexShaders.end())
		return m_vertexShaders[token]->VS;

	return nullptr;
}

ID3D11InputLayout* CradleFileManager::GetInputLayout(wchar_t* token)
{
	if (m_vertexShaders.find(token) != m_vertexShaders.end())
		return m_vertexShaders[token]->IL;

	return nullptr;
}


bool CradleFileManager::AddMesh(CradleResource* mesh)
{
	
	if (m_meshes.find(mesh->GetName()) != m_meshes.end())
		return false;

	m_meshes[mesh->GetName()] = mesh;

}

CradleResource* CradleFileManager::GetMesh(std::wstring token)
{
	if (m_meshes.find(token) != m_meshes.end())
		return (CradleMesh*)m_meshes[token];

	return nullptr;
}


bool CradleFileManager::LoadMeshes()
{
	// /*
	wprintf_s(L"STARTING LOAD MESHES\n");
	char* fileToLoad = "D:\\Dropbox\\Dropbox\\_DirectX\\Source\\Cradle\\Debug\\Content\\Meshes\\cube.obj";

	ObjModel om;

	// ********************************** CUBE *******************************
	bool b = om.LoadOBJ(fileToLoad);

	float* verts = om.GetUniqueVerts();
	float* normals = om.GetUniqueNormals();

	CradleVertexDeclarations::VertexPosTex* temp = new CradleVertexDeclarations::VertexPosTex[om.GetTotalUniqueVerts()];

	int i = 0;


	for (i = 0; i < om.GetTotalUniqueVerts(); i++)
	{
		temp[i].pos.x = verts[i * 3 + 0];
		temp[i].pos.y = verts[i * 3 + 1];
		temp[i].pos.z = verts[i * 3 + 2];

		temp[i].normal.x = normals[i * 3 + 0];
		temp[i].normal.y = normals[i * 3 + 1];
		temp[i].normal.z = normals[i * 3 + 2];

		wprintf_s(L"CubeNormal(%f, %f, %f)\n", temp[i].normal.x, temp[i].normal.y, temp[i].normal.z);
	}

	DWORD* indices = new DWORD[om.GetTotalIndices()];

	int* id = om.GetIndices();

	for (i = 0; i < om.GetTotalIndices(); i++)
	{
		indices[i] = id[i];
	}
	
	
	CradleMesh* mesh = new CradleMesh(L"cube", this, temp, om.GetTotalUniqueVerts() / 3, indices, om.GetTotalIndices());
	
	wprintf_s(L"Mesh created with %d verts\n", om.GetTotalVerts());

	om.Release();

	// */

	// ********************************** CYLINDER *******************************
	

	fileToLoad = "D:\\Dropbox\\Dropbox\\_DirectX\\Source\\Cradle\\Debug\\Content\\Meshes\\cylinder.obj";

	om.LoadOBJ(fileToLoad);

	verts = om.GetUniqueVerts();
	normals = om.GetNormals();

	temp = new CradleVertexDeclarations::VertexPosTex[om.GetTotalUniqueVerts()];

	for (int i = 0; i < om.GetTotalUniqueVerts(); i++)
	{
		temp[i].pos.x = verts[i * 3 + 0];
		temp[i].pos.y = verts[i * 3 + 1];
		temp[i].pos.z = verts[i * 3 + 2];

		temp[i].normal.x = normals[i * 3 + 0];
		temp[i].normal.y = normals[i * 3 + 1];
		temp[i].normal.z = normals[i * 3 + 2];
	}

	indices = new DWORD[om.GetTotalIndices()];

	id = om.GetIndices();

	for (i = 0; i < om.GetTotalIndices(); i++)
	{
		indices[i] = id[i];
	}

	mesh = new CradleMesh(L"cylinder", this, temp, om.GetTotalUniqueVerts() / 3, indices, om.GetTotalIndices());

	om.Release();

	
	// ********************************** CONE *******************************

	
	fileToLoad = "D:\\Dropbox\\Dropbox\\_DirectX\\Source\\Cradle\\Debug\\Content\\Meshes\\cone.obj";

	om.LoadOBJ(fileToLoad);

	verts = om.GetUniqueVerts();

	temp = new CradleVertexDeclarations::VertexPosTex[om.GetTotalUniqueVerts()];

	for (int i = 0; i < om.GetTotalUniqueVerts(); i++)
	{
		temp[i].pos.x = verts[i * 3 + 0];
		temp[i].pos.y = verts[i * 3 + 1];
		temp[i].pos.z = verts[i * 3 + 2];

		temp[i].normal.x = normals[i * 3 + 0];
		temp[i].normal.y = normals[i * 3 + 1];
		temp[i].normal.z = normals[i * 3 + 2];
	}

	indices = new DWORD[om.GetTotalIndices()];

	id = om.GetIndices();

	for (i = 0; i < om.GetTotalIndices(); i++)
	{
		indices[i] = id[i];
	}

	mesh = new CradleMesh(L"cone", this, temp, om.GetTotalUniqueVerts() / 3, indices, om.GetTotalIndices());
	om.Release();
	

	// */

	return true;
}

bool CradleFileManager::LoadShaders()
{
	ID3DBlob* shaderData = nullptr;
	HRESULT hr = E_FAIL;
	ID3D11PixelShader* ps = nullptr;
	ID3D11VertexShader* vs = nullptr;



	wprintf_s(L"Beginning to load Shaders\n");

	wchar_t file[1024];

	wcscpy(file, m_resourcePath);
	wcscat(file, L"SolidColor.hlsl");

	wprintf_s(L"Loading Shaders: %s\n", file);
	


	// LOADING PIXEL SHADER
	hr = m_cd3d->CompileShader(file, "PSMain", "ps_4_0", &shaderData);

	wprintf_s(L"Compiled PS Shader\n");

	if (FAILED(hr))
	{
		wprintf(L"Could not compile PixelShader: %x\n", hr);
		return false;
	}
	else
	{
		hr = m_cd3d->m_d3dDevice->CreatePixelShader(shaderData->GetBufferPointer(), shaderData->GetBufferSize(), 0, &ps);

		wprintf_s(L"Created PixelShader\n");

		ReleaseCOM(shaderData);
	}
	wprintf_s(L"Released shaderData\n");

	if (FAILED(hr))
	{
		wprintf(L"Could not create PixelShader\n");
		return false;
	}

	m_pixelShaders[L"SolidColor"] = ps;
	ps = nullptr;
	
	// END PIXEL SHADER





	// LOADING VERTEX SHADER

	Foo* foo = new Foo();
	hr = m_cd3d->CompileShader(file, "VSMain", "vs_4_0", &shaderData);

	if (FAILED(hr))
	{
		wprintf(L"Could not compile VertexShader\n");
		return false;
	}

	hr = m_cd3d->m_d3dDevice->CreateVertexShader(shaderData->GetBufferPointer(), shaderData->GetBufferSize(), 0, &vs);

	if (FAILED(hr))
	{
		wprintf(L"Could not create VertexShader\n");
		return false;
	}

	foo->VS = vs;

	/*
	D3D11_INPUT_ELEMENT_DESC vertexLayout[] =
	{
		{
			"POSITION"						// Semantic name
			, 0								// Semantic index
			, DXGI_FORMAT_R32G32B32_FLOAT	// Format
			, 0								// Input slot
			, 0								// Aligned byte offset
			, D3D11_INPUT_PER_VERTEX_DATA	// Input slot class
			, 0								// Instance data step rate
		},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{
			"TEXCOORD",
			1,
			DXGI_FORMAT_R32G32B32_FLOAT,
			1,
			0,
			D3D11_INPUT_PER_INSTANCE_DATA,
			1
		}
	};


	unsigned int totalLayoutElements = ARRAYSIZE(vertexLayout);

	*/

	hr = m_cd3d->m_d3dDevice->CreateInputLayout(CradleVertexDeclarations::VertexPositionTexture
		, CradleVertexDeclarations::VertexPositionTextureNumElements
		, shaderData->GetBufferPointer()
		, shaderData->GetBufferSize()
		, &foo->IL);

	m_vertexShaders[L"SolidColor"] = foo;
	// vs = nullptr;

	for (const auto& item : m_vertexShaders)
	{
		wprintf_s(L"First: %s\n", item.first);
	}

	ReleaseCOM(shaderData);
	wprintf_s(L"All ShadersLoaded\n");
	// END VERTEX SHADER

	return true;
}

bool CradleFileManager::GetFilenames()
{
	wchar_t* resourcePath = L"D:\\Dropbox\\Dropbox\\_DirectX\\Source\\Cradle\\Debug\\Content\\Shaders\\*.*";
	
	WIN32_FIND_DATA data;
	HANDLE file;

	file = FindFirstFile(resourcePath, &data);

	if (file == INVALID_HANDLE_VALUE)
	{
		wprintf_s(L"Invalid handle");
	}
	else
	{
		do
		{
			wprintf_s(L"Filename: %s\r\n", data.cFileName);
		} while (FindNextFile(file, &data));
		

		FindClose(file);
	}
	
	return true;
}

void CradleFileManager::DrawMesh(std::wstring name, DirectX::XMFLOAT4X4* matrix)
{
	m_meshesToDraw[name].push_back(matrix);
}

void CradleFileManager::Draw()
{
	// Draw actually draw all queued up motherfuckers

	std::list<std::wstring>::iterator it;
}