#ifndef _OBJMODEL_
#define _OBJMODEL_

class ObjModel
{
public:
	__declspec(dllexport) ObjModel();
	__declspec(dllexport) ~ObjModel();

	__declspec(dllexport) void Release();
	__declspec(dllexport) bool LoadOBJ(char* fileName);

	float* GetVertices() { return m_vertices; }
	float* GetNormals() { return m_normals; }
	int* GetIndices() { return m_indices; }
	float* GetUniqueVerts() { return m_uniqueVerts; }
	__declspec(dllexport) float* GetTexCoords() { return m_texCoords; }
	__declspec(dllexport) int GetTotalVerts() { return m_totalVerts; }

	int GetTotalIndices() { return m_totalIndices; }
	int GetTotalUniqueVerts() { return m_totalUniqueVerts; }

	float* GetUniqueNormals() { return m_uniqueNormals; }

private:
	float* m_vertices;
	float* m_normals;
	float* m_texCoords;
	int m_totalVerts;
	int m_totalIndices;


	int m_totalUniqueVerts;
	float* m_uniqueVerts;
	int* m_indices;

	float* m_uniqueNormals;
};



#endif