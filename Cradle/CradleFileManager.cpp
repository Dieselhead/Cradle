#include "CradleFileManager.h"
#include "CradleD3D.h"
#include "Macros.h"
#include "CradleResource.h"
#include "ObjModel.h"
#include "CradleMesh.h"
#include "CradleVertexLayouts.h"
#include <vector>
#include <thread>

CradleFileManager::CradleFileManager(CradleD3D* cd3d)
{
	m_cd3d = cd3d;

	m_instanceDataBufferSize = 0;
	m_instanceData = nullptr;
	m_instanceBuffer = nullptr;
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


	m_instanceDataBufferSize = 0;
	
	if (m_instanceData) delete[] m_instanceData;
	m_instanceData = nullptr;

	if (m_instanceBuffer) m_instanceBuffer->Release();
	m_instanceBuffer = nullptr;


	for (const auto &data : m_meshInstanceData)
	{
		if (data.second)
		{
			if (data.second->Buffer)
				data.second->Buffer->Release();
			data.second->Buffer = nullptr;

			if (data.second->InstanceData)
				delete[] data.second->InstanceData;
			data.second->InstanceData = nullptr;

			delete data.second;
		}
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

void CradleFileManager::LoadMeshFromObj(const wchar_t* fileName, const wchar_t* assetName)
{
	ObjModel om;
	bool b = om.LoadOBJ(fileName);
	int i = 0;


	

	float* positionData = om.GetUniqueVerts();
	float* normalData = om.GetUniqueNormals();
	float* texCoordData = om.GetUniqueTexCoords();

	int totalPositions = om.GetTotalUniqueVerts();
	int totalNormals = om.GetTotalUniqueNormals();
	int totalTexCoords = om.GetTotalTexCoords();

	int* positionIndices = om.GetIndices();
	int* normalIndices = om.GetNormalIndices();
	int* texCoordIndices = om.GetTexCoordIndices();

	int numRawIndices = om.GetTotalIndices();

	CradleVertexDeclarations::VertexPosTex* finalVertexData = nullptr;
	DWORD* finalIndexData = new DWORD[numRawIndices];


	struct PosNormTex
	{
		int pos;
		int norm;
		int tex;
	};


	CradleVertexDeclarations::VertexPosTex* newVertex = nullptr;
	std::vector<CradleVertexDeclarations::VertexPosTex*> UniqueVerts;
	
	PosNormTex* newIndexPair = nullptr;
	std::vector<PosNormTex*> UniqueIndexPairs;




	int doubleIndex = -1;

	for (i = 0; i < numRawIndices; i++)
	{
		doubleIndex = -1;

		for (int j = 0; j < UniqueIndexPairs.size(); j++)
		{
			if (UniqueIndexPairs[j]->pos == positionIndices[i]
				&& UniqueIndexPairs[j]->norm == normalIndices[i]
				&& UniqueIndexPairs[j]->tex == texCoordIndices[i])
			{
				doubleIndex = j;
				break;
			}
		}


		if (doubleIndex >= 0)
		{

			finalIndexData[i] = doubleIndex;
		}
		else
		{
			newVertex = new CradleVertexDeclarations::VertexPosTex();

			newVertex->pos.x = positionData[positionIndices[i] * 3 + 0];
			newVertex->pos.y = positionData[positionIndices[i] * 3 + 1];
			newVertex->pos.z = positionData[positionIndices[i] * 3 + 2];

			newVertex->normal.x = normalData[normalIndices[i] * 3 + 0];
			newVertex->normal.y = normalData[normalIndices[i] * 3 + 1];
			newVertex->normal.z = normalData[normalIndices[i] * 3 + 2];

			newVertex->tex0.x = texCoordData[texCoordIndices[i] * 2 + 0];
			newVertex->tex0.y = texCoordData[texCoordIndices[i] * 2 + 1];

			UniqueVerts.push_back(newVertex);



			newIndexPair = new PosNormTex();
			newIndexPair->pos = positionIndices[i];
			newIndexPair->norm = normalIndices[i];
			newIndexPair->tex = texCoordIndices[i];

			UniqueIndexPairs.push_back(newIndexPair);


			finalIndexData[i] = UniqueVerts.size() - 1;
		}
	}


	finalVertexData = new CradleVertexDeclarations::VertexPosTex[UniqueVerts.size()];
	for (i = 0; i < UniqueVerts.size(); i++)
	{
		finalVertexData[i] = *UniqueVerts[i];
	}

	





	CradleMesh* mesh = new CradleMesh(assetName, this, finalVertexData, UniqueVerts.size(), finalIndexData, numRawIndices);


	for (i = 0; i < UniqueIndexPairs.size(); i++)
	{
		delete UniqueIndexPairs[i];
	}

	wprintf_s(L"Mesh created with %d unique vertices\n", UniqueVerts.size());
	om.Release();


	m_meshInstanceData[assetName] = new MeshInstanceData();
	IncreaseInstanceDataSize(assetName, 1);
}

bool CradleFileManager::LoadMeshes()
{
	LoadMeshFromObj(L".\\Content\\Meshes\\monkey.obj", L"monkey");
	LoadMeshFromObj(L".\\Content\\Meshes\\monkey_smooth.obj", L"monkey_smooth");
	LoadMeshFromObj(L".\\Content\\Meshes\\cube.obj", L"cube");
	LoadMeshFromObj(L".\\Content\\Meshes\\cylinder.obj", L"cylinder");
	
	/*
	wprintf_s(L"STARTING LOAD MESHES\n");
	char* fileToLoad = ".\\Content\\Meshes\\monkey_smooth.obj";

	ObjModel om;
	int i = 0;

	// ********************************** CUBE *******************************
	bool b = om.LoadOBJ(fileToLoad);

	CradleVertexDeclarations::VertexPosTex* temp = nullptr; 

	float* verts = om.GetUniqueVerts();
	float* normals = om.GetUniqueNormals();
	float* tex = om.GetUniqueTexCoords();

	int totalVerts = om.GetTotalUniqueVerts();
	int totalNormals = om.GetTotalUniqueNormals();
	int totalTex = om.GetTotalTexCoords();

	wprintf_s(L"############################################################\n");

	for (i = 0; i < totalVerts / 3; i++)
	{
		wprintf_s(L"[%d] Vertice: (%f, %f, %f)\n", i, verts[i * 3 + 0], verts[i * 3 + 1], verts[i * 3 + 2]);
	}

	wprintf_s(L"--------------------------------------------------------\n");

	for (i = 0; i < totalNormals / 3; i++)
	{
		wprintf_s(L"[%d] Normal: (%f, %f, %f)\n", i, normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]);
	}

	wprintf_s(L"--------------------------------------------------------\n");

	for (i = 0; i < totalTex / 2; i++)
	{
		wprintf_s(L"[%d] TexCoord: (%f, %f)\n", i, tex[i * 2 + 0], tex[i * 2 + 1]);
	}





	

	int* id = om.GetIndices();
	int* normalIndex = om.GetNormalIndices();
	int* texIndex = om.GetTexCoordIndices();

	int totalFaces = om.GetTotalIndices();


	wprintf_s(L"--------------------------------------------------------\n");

	for (i = 0; i < totalFaces; i += 3)
	{
		wprintf_s(L"Face[%d]: %d/%d/%d %d/%d/%d %d/%d/%d\n", i,
			(id[i] + 1), (texIndex[i] + 1), (normalIndex[i] + 1),
			(id[i + 1] + 1), (texIndex[i + 1] + 1), (normalIndex[i + 1] + 1),
			(id[i + 2] + 1), (texIndex[i + 2] + 1), (normalIndex[i + 2] + 1));
	}




	wprintf_s(L"############################################################\n\n");
	
	
	// Loop through all indices
	// For each index
		// If index combination exists
			// Add that index to index list
		// If index does not exist
			// grab actual values from arrays for Pos/Norm/Tex
			// Add values to PosTexNorm array
			// Add new index to index list

	CradleVertexDeclarations::VertexPosTex* newVertex = nullptr;
	std::vector<CradleVertexDeclarations::VertexPosTex*> UniqueVerts;
	struct PosNormTex
	{
		int pos;
		int norm;
		int tex;
	};

	std::vector<PosNormTex*> UniqueIndexPairs;
	PosNormTex* newIndexPair = nullptr;
	
	DWORD* idx = new DWORD[totalFaces];

	int doubleIndex = -1;

	for (i = 0; i < totalFaces; i++)
	{
		doubleIndex = -1;

		for (int j = 0; j < UniqueIndexPairs.size(); j++)
		{
			if (UniqueIndexPairs[j]->pos == id[i]
				&& UniqueIndexPairs[j]->norm == normalIndex[i]
				&& UniqueIndexPairs[j]->tex == texIndex[i])
			{
				doubleIndex = j;
				break;
			}
		}


		if (doubleIndex >= 0)
		{

			idx[i] = doubleIndex;
		}
		else
		{
			newVertex = new CradleVertexDeclarations::VertexPosTex();

			newVertex->pos.x = verts[id[i] * 3 + 0];
			newVertex->pos.y = verts[id[i] * 3 + 1];
			newVertex->pos.z = verts[id[i] * 3 + 2];

			newVertex->normal.x = normals[normalIndex[i] * 3 + 0];
			newVertex->normal.y = normals[normalIndex[i] * 3 + 1];
			newVertex->normal.z = normals[normalIndex[i] * 3 + 2];

			newVertex->tex0.x = tex[texIndex[i] * 2 + 0];
			newVertex->tex0.y = tex[texIndex[i] * 2 + 1];

			UniqueVerts.push_back(newVertex);



			newIndexPair = new PosNormTex();
			newIndexPair->pos = id[i];
			newIndexPair->norm = normalIndex[i];
			newIndexPair->tex = texIndex[i];

			UniqueIndexPairs.push_back(newIndexPair);
			

			idx[i] = UniqueVerts.size() - 1;
		}
	}

	
	temp = new CradleVertexDeclarations::VertexPosTex[UniqueVerts.size()];
	for (i = 0; i < UniqueVerts.size(); i++)
	{
		temp[i] = *UniqueVerts[i];
	}

	for (i = 0; i < UniqueIndexPairs.size(); i++)
	{
		delete UniqueIndexPairs[i];
	}



	
	
	CradleMesh* mesh = new CradleMesh(L"monkey_smooth", this, temp, UniqueVerts.size(), idx, totalFaces);
	
	wprintf_s(L"Mesh created with %d unique vertices\n", UniqueVerts.size());


	om.Release();

	// */

	// ********************************** CYLINDER *******************************
	/*

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
	wcscat(file, L"Shaders\\SolidColor.hlsl");

	wprintf_s(L"Loading Shaders: %s\n", file);
	


	// LOADING PIXEL SHADER
	hr = m_cd3d->CompileShader(file, "PSMain", "ps_4_0", &shaderData);

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
	wchar_t* resourcePath = L"Content\\Shaders\\*.*";
	
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

void CradleFileManager::DrawMesh(DirectX::XMFLOAT4X4* matrix)
{
	m_meshesToDraw[L"monkey"].push_back(matrix);
}

void CradleFileManager::Draw()
{

#ifndef _USE_ONE_INSTANCEBUFFER_
	
	std::list<DirectX::XMFLOAT4X4*>* list = nullptr;


	for (const auto& item : m_meshesToDraw)
	{
		list = (std::list<DirectX::XMFLOAT4X4*>*)(&item.second);
		MeshInstanceData* mid = m_meshInstanceData[item.first];


		if (mid->BufferSize < list->size())
		{
			IncreaseInstanceDataSize(item.first, list->size());
		}

		std::list<DirectX::XMFLOAT4X4*>::iterator it = list->begin();
		int count = 0;


		for (; it != list->end(); it++)
		{
			// put these matrices in the instance data array
			DirectX::XMFLOAT4X4* m = (DirectX::XMFLOAT4X4*)(*it);

			mid->InstanceData[count].world0 = DirectX::XMFLOAT4(m->m[0]);
			mid->InstanceData[count].world1 = DirectX::XMFLOAT4(m->m[1]);
			mid->InstanceData[count].world2 = DirectX::XMFLOAT4(m->m[2]);
			mid->InstanceData[count].world3 = DirectX::XMFLOAT4(m->m[3]);
			count++;
		}
		

		D3D11_MAPPED_SUBRESOURCE ms;
		m_cd3d->m_d3dContext->Map(mid->Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		memcpy(ms.pData, mid->InstanceData, sizeof(CradleVertexDeclarations::InstanceData) * list->size());
		m_cd3d->m_d3dContext->Unmap(mid->Buffer, 0);


		CradleMesh* mesh = (CradleMesh*)GetMesh(item.first);
		ID3D11Buffer* vb_ = mesh->GetVertexBuffer();
		ID3D11Buffer* indexBuffer = mesh->GetIndexBuffer();
		m_cd3d->m_d3dContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		unsigned int strides[2];
		unsigned int offsets[2];
		ID3D11Buffer* bufferPointers[2];

		strides[0] = sizeof(CradleVertexDeclarations::VertexPosTex);
		strides[1] = sizeof(CradleVertexDeclarations::InstanceData);

		offsets[0] = 0;
		offsets[1] = 0;

		bufferPointers[0] = vb_;
		bufferPointers[1] = mid->Buffer;


		m_cd3d->m_d3dContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

		m_cd3d->m_d3dContext->DrawIndexedInstanced(mesh->GetNumIndices(), list->size(), 0, 0, 0);

	}

	m_meshesToDraw.clear();
	

#else
	
	// Draw actually draw all queued up motherfuckers
	std::list<DirectX::XMFLOAT4X4*>* list = nullptr;


	for (const auto& item : m_meshesToDraw)
	{
		list = (std::list<DirectX::XMFLOAT4X4*>*)(&item.second);

		if (m_instanceDataBufferSize < list->size())
		{
			IncreaseInstanceDataSize(list->size());
		}

		std::list<DirectX::XMFLOAT4X4*>::iterator it = list->begin();
		int count = 0;


		for (; it != list->end(); it++)
		{
			// put these matrices in the instance data array
			DirectX::XMFLOAT4X4* m = (DirectX::XMFLOAT4X4*)(*it);

			m_instanceData[count].world0 = DirectX::XMFLOAT4(m->m[0]);
			m_instanceData[count].world1 = DirectX::XMFLOAT4(m->m[1]);
			m_instanceData[count].world2 = DirectX::XMFLOAT4(m->m[2]);
			m_instanceData[count].world3 = DirectX::XMFLOAT4(m->m[3]);
			count++;
		}
		
		

		D3D11_MAPPED_SUBRESOURCE ms;
		m_cd3d->m_d3dContext->Map(m_instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		memcpy(ms.pData, m_instanceData, sizeof(CradleVertexDeclarations::InstanceData) * list->size());
		m_cd3d->m_d3dContext->Unmap(m_instanceBuffer, 0);


		CradleMesh* mesh = (CradleMesh*)GetMesh(item.first);
		ID3D11Buffer* vb_ = mesh->GetVertexBuffer();
		ID3D11Buffer* indexBuffer = mesh->GetIndexBuffer();
		m_cd3d->m_d3dContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		unsigned int strides[2];
		unsigned int offsets[2];
		ID3D11Buffer* bufferPointers[2];

		strides[0] = sizeof(CradleVertexDeclarations::VertexPosTex);
		strides[1] = sizeof(CradleVertexDeclarations::InstanceData);

		offsets[0] = 0;
		offsets[1] = 0;

		bufferPointers[0] = vb_;
		bufferPointers[1] = m_instanceBuffer;


		m_cd3d->m_d3dContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
		
		m_cd3d->m_d3dContext->DrawIndexedInstanced(mesh->GetNumIndices(), list->size(), 0, 0, 0);

	}

	m_meshesToDraw.clear();
	
#endif
}


void CradleFileManager::IncreaseInstanceDataSize(std::wstring mesh, int newSize)
{	
	MeshInstanceData* mid = m_meshInstanceData[mesh];

	// Release current instanceBuffer
	if (mid->Buffer) mid->Buffer->Release();
	mid->Buffer = nullptr;

	// Keep current instanceData in temp variable
	if (mid->BufferSize > 0)
	{
		CradleVertexDeclarations::InstanceData* temp = mid->InstanceData;

		// Create new instanceData array sizeof CurrentSize+1
		mid->InstanceData = new CradleVertexDeclarations::InstanceData[newSize];

		// copy memory over from temp array
		// memcpy(mid->InstanceData, temp, sizeof(mid->InstanceData[0]) * mid->BufferSize);

		// Delete temp array
		delete[] temp;
	}
	else
	{

		mid->InstanceData = new CradleVertexDeclarations::InstanceData[newSize];
	}

	// Create new instancebuffer
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.ByteWidth = sizeof(CradleVertexDeclarations::InstanceData) * (newSize);

	D3D11_SUBRESOURCE_DATA subdata;
	ZeroMemory(&subdata, sizeof(D3D11_SUBRESOURCE_DATA));
	subdata.pSysMem = mid->InstanceData;
	subdata.SysMemPitch = 0;
	subdata.SysMemSlicePitch = 0;


	HRESULT hr = m_cd3d->m_d3dDevice->CreateBuffer(&desc, &subdata, &mid->Buffer);

	

	// Increase size by one
	mid->BufferSize = newSize;

	if (FAILED(hr))
	{
		assert(false);
	}
	else
	{
		wprintf_s(L"Successfully created new instance buffer\n");
	}

	wprintf_s(L"I have increased the size of the instance data buffer to %d for %s\n", newSize, mesh.c_str());
}

void CradleFileManager::IncreaseInstanceDataSize(int newSize)
{
	wprintf_s(L"I have increased the size of the instance data buffer to %d\n", newSize);

	// Release current instanceBuffer
	if (m_instanceBuffer) m_instanceBuffer->Release();
	m_instanceBuffer = nullptr;

	// Keep current instanceData in temp variable
	CradleVertexDeclarations::InstanceData* temp = m_instanceData;
	
	// Create new instanceData array sizeof CurrentSize+1
	m_instanceData = new CradleVertexDeclarations::InstanceData[newSize];

	// copy memory over from temp array
	memcpy(m_instanceData, temp, sizeof(m_instanceData[0]) * m_instanceDataBufferSize);

	// Delete temp array
	delete[] temp;

	// Create new instancebuffer
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.ByteWidth = sizeof(CradleVertexDeclarations::InstanceData) * (newSize);

	D3D11_SUBRESOURCE_DATA subdata;
	ZeroMemory(&subdata, sizeof(D3D11_SUBRESOURCE_DATA));
	subdata.pSysMem = m_instanceData;
	subdata.SysMemPitch = 0;
	subdata.SysMemSlicePitch = 0;


	HRESULT hr = m_cd3d->m_d3dDevice->CreateBuffer(&desc, &subdata, &m_instanceBuffer);

	if (FAILED(hr))
	{
		assert(false);
	}

	// Increase size by one
	m_instanceDataBufferSize = newSize;

	wprintf_s(L"I have increased the size of the instance data buffer to %d\n", newSize);
}