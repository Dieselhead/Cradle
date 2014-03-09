#include "ObjModel.h"
#include "TokenStream.h"
#include <fstream>
#include <vector>

ObjModel::ObjModel()
{
	m_texCoords = 0;
	m_vertices = 0;
	m_normals = 0;

	m_uniqueVerts = 0;
	m_indices = 0;
	m_normalIndices = 0;

	m_uniqueNormals = 0;
	m_uniqueTexCoords = 0;
}

ObjModel::~ObjModel()
{

}

void ObjModel::Release()
{
	if (m_vertices)	delete[] m_vertices;
	m_vertices = nullptr;

	if (m_normals) delete[] m_normals;
	m_normals = nullptr;

	if (m_texCoords) delete[] m_texCoords;
	m_texCoords = nullptr;


	if (m_indices) delete[] m_indices;
	m_indices = nullptr;

	if (m_normalIndices) delete[] m_normalIndices;
	m_normalIndices = nullptr;

	if (m_texIndices) delete[] m_texIndices;
	m_texIndices = nullptr;


	if (m_uniqueVerts) delete[] m_uniqueVerts;
	m_uniqueVerts = nullptr;

	if (m_uniqueNormals) delete[] m_uniqueNormals;
	m_uniqueNormals = nullptr;

	if (m_uniqueTexCoords) delete[] m_uniqueTexCoords;
	m_uniqueTexCoords = nullptr;
}

bool ObjModel::LoadOBJ(const wchar_t* fileName)
{
	wprintf_s(L"Loading Obj from: %s\n", fileName);

	std::ifstream fileStream;
	int fileSize = 0;

	fileStream.open(fileName, std::ifstream::in);

	wprintf_s(L"fileStream.open()\n");

	if (fileStream.is_open() == false)
		return false;


	wprintf_s(L"fileStream.is_open()\n");

	fileStream.seekg(0, std::ios::end);
	fileSize = (int)fileStream.tellg();
	fileStream.seekg(0, std::ios::beg);

	if (fileSize <= 0)
		return false;

	char* buffer = new char[fileSize];

	if (buffer == 0)
		return false;

	memset(buffer, '\0', fileSize);

	TokenStream tokenStream;
	TokenStream lineStream;
	TokenStream faceStream;
	std::string tempLine;
	std::string token;

	fileStream.read(buffer, fileSize);
	tokenStream.SetTokenStream(buffer);

	delete[] buffer;

	tokenStream.ResetStream();

	std::vector<float> verts, norms, texC;
	std::vector<int> faces;

	char lineDelimiters[2] = { '\n', ' ' };

	while (tokenStream.MoveToNextLine(&tempLine))
	{
		lineStream.SetTokenStream((char*)tempLine.c_str());
		tokenStream.GetNextToken(0, 0, 0);

		if (!lineStream.GetNextToken(&token, lineDelimiters, 2))
			continue;

		if (strcmp(token.c_str(), "v") == 0)
		{
			lineStream.GetNextToken(&token, lineDelimiters, 2);
			verts.push_back((float)atof(token.c_str()));

			lineStream.GetNextToken(&token, lineDelimiters, 2);
			verts.push_back((float)atof(token.c_str()));

			lineStream.GetNextToken(&token, lineDelimiters, 2);
			verts.push_back((float)atof(token.c_str()));
		}
		else if (strcmp(token.c_str(), "vn") == 0)
		{
			lineStream.GetNextToken(&token, lineDelimiters, 2);
			norms.push_back((float)atof(token.c_str()));

			lineStream.GetNextToken(&token, lineDelimiters, 2);
			norms.push_back((float)atof(token.c_str()));

			lineStream.GetNextToken(&token, lineDelimiters, 2);
			norms.push_back((float)atof(token.c_str()));
		}
		else if (strcmp(token.c_str(), "vt") == 0)
		{
			lineStream.GetNextToken(&token, lineDelimiters, 2);
			texC.push_back((float)atof(token.c_str()));

			lineStream.GetNextToken(&token, lineDelimiters, 2);
			texC.push_back((float)atof(token.c_str()));
		}
		else if (strcmp(token.c_str(), "f") == 0)
		{
			char faceTokens[3] = { '\n', ' ', '/' };
			std::string faceIndex;

			faceStream.SetTokenStream((char*)tempLine.c_str());
			faceStream.GetNextToken(0, 0, 0);

			for (int i = 0; i < 3; i++)
			{
				faceStream.GetNextToken(&faceIndex, faceTokens, 3);
				faces.push_back((int)atoi(faceIndex.c_str()));

				faceStream.GetNextToken(&faceIndex, faceTokens, 3);
				faces.push_back((int)atoi(faceIndex.c_str()));

				faceStream.GetNextToken(&faceIndex, faceTokens, 3);
				faces.push_back((int)atoi(faceIndex.c_str()));
			}
		}
		else if (strcmp(token.c_str(), "#") == 0)
		{
			int a = 0;
			int b = a;
		}

		token[0] = '\0';
	}


	int vIndex = 0, nIndex = 0, tIndex = 0, iIndex = 0;
	int numFaces = (int)faces.size() / 9;

	m_totalVerts = numFaces * 3;

	m_vertices = new float[m_totalVerts * 3];




	wprintf_s(L"FACES.SIZE: %d\nNUM FACES: %d\n", faces.size(), numFaces);
	// Indices from faces
	// There will be doubles in these
	// Indices should be in range of 0 -> m_totalUniqueVerts-1
	m_indices = new int[m_totalVerts];
	m_normalIndices = new int[m_totalVerts];
	m_texIndices = new int[m_totalVerts];

	m_totalIndices = m_totalVerts;

	m_uniqueVerts = new float[verts.size()];
	m_totalUniqueVerts = verts.size();

	m_uniqueNormals = new float[norms.size()];
	m_totalUniqueNormals = norms.size();

	m_uniqueTexCoords = new float[texC.size()];
	m_totalUniqueTexCoords = texC.size();

	for (int i = 0; i < norms.size(); i++)
	{
		m_uniqueNormals[i] = norms[i];
	}

	for (int i = 0; i < verts.size(); i++)
	{
		m_uniqueVerts[i] = verts[i];
	}

	for (int i = 0; i < texC.size(); i++)
	{
		m_uniqueTexCoords[i] = texC[i];
	}





	if ((int)norms.size() != 0)
	{
		m_normals = new float[m_totalVerts * 3];
	}

	if ((int)texC.size() != 0)
	{
		m_texCoords = new float[m_totalVerts * 2];
	}

	for (int f = 0; f < (int)faces.size(); f += 3)
	{
		

		m_indices[iIndex] = faces[f]-1;
		m_texIndices[iIndex] = faces[f + 1] - 1;
		m_normalIndices[iIndex] = faces[f + 2] - 1;

		iIndex++;

		m_vertices[vIndex + 0] = verts[(faces[f + 0] - 1) * 3 + 0];
		m_vertices[vIndex + 1] = verts[(faces[f + 0] - 1) * 3 + 1];
		m_vertices[vIndex + 2] = verts[(faces[f + 0] - 1) * 3 + 2];
		
		vIndex += 3;

		if (m_texCoords)
		{
			m_texCoords[tIndex + 0] = texC[(faces[f + 1] - 1) * 2 + 0];
			m_texCoords[tIndex + 1] = texC[(faces[f + 1] - 1) * 2 + 1];
			tIndex += 2;
		}

		if (m_normals)
		{
			m_normals[nIndex + 0] = norms[(faces[f + 2] - 1) * 3 + 0];
			m_normals[nIndex + 1] = norms[(faces[f + 2] - 1) * 3 + 1];
			m_normals[nIndex + 2] = norms[(faces[f + 2] - 1) * 3 + 2];
			nIndex += 3;
		}
	}

	verts.clear();
	norms.clear();
	texC.clear();
	faces.clear();


	return true;
}