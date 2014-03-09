#ifndef _OBJMODEL_
#define _OBJMODEL_

class ObjModel
{
public:
	__declspec(dllexport) ObjModel();
	__declspec(dllexport) ~ObjModel();

	__declspec(dllexport) void Release();
	__declspec(dllexport) bool LoadOBJ(const wchar_t* fileName);

	float* GetVertices() { return m_vertices; }
	float* GetNormals() { return m_normals; }
	__declspec(dllexport) float* GetTexCoords() { return m_texCoords; }
	__declspec(dllexport) int GetTotalVerts() { return m_totalVerts; }





	int GetTotalUniqueVerts() { return m_totalUniqueVerts; }
	float* GetUniqueVerts() { return m_uniqueVerts; }


	int GetTotalUniqueNormals() { return m_totalUniqueNormals; }
	float* GetUniqueNormals() { return m_uniqueNormals; }


	int GetTotalTexCoords() { return m_totalUniqueTexCoords; }
	float* GetUniqueTexCoords() { return m_uniqueTexCoords; }



	
	int GetTotalIndices() { return m_totalIndices; }
	int* GetIndices() { return m_indices; }
	int* GetNormalIndices() { return m_normalIndices; }
	int* GetTexCoordIndices() { return m_texIndices; }

	

private:
	float* m_vertices;
	float* m_normals;
	float* m_texCoords;
	int m_totalVerts;


	int m_totalUniqueVerts;
	float* m_uniqueVerts;

	int m_totalIndices;
	int* m_indices;
	int* m_normalIndices;
	int* m_texIndices;

	float* m_uniqueNormals;
	int m_totalUniqueNormals;

	float* m_uniqueTexCoords;
	int m_totalUniqueTexCoords;
};



#endif