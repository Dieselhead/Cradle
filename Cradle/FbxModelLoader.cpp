#include "FbxModel.h"
#include "FbxModelLoader.h"
#include <fstream>

FbxModelLoader::FbxModelLoader()
{

}

FbxModelLoader::~FbxModelLoader()
{
	
	if (m_importer)
		m_importer->Destroy();

	if (m_manager)
		m_manager->Destroy();

}

int FbxModelLoader::Initialize()
{
	m_manager = FbxManager::Create();

	if (!m_manager)
	{
		
		// MessageBoxA(g_hWnd, "FbxManager failed to create", "fail", MB_OK);
		return -1;
	}
	FbxIOSettings* ios = FbxIOSettings::Create(m_manager, IOSROOT);
	if (!ios)
		return -2;
	else
		m_manager->SetIOSettings(ios);

	m_importer = FbxImporter::Create(m_manager, "");
	if (!m_importer)
		return -3;

	return 1;
}

Model* FbxModelLoader::LoadModel(const char* fileName)
{
	Model* model = nullptr;

	if (!m_importer->Initialize(fileName, -1, m_manager->GetIOSettings()))
	{
		return nullptr;
	}

	FbxScene* lScene = FbxScene::Create(m_manager, "myscene");
	m_importer->Import(lScene);


	FbxNode* lRootNode = lScene->GetRootNode();

	if (lRootNode)
	{
		if (lRootNode->GetChildCount() > 0)
		{
			FbxNode* lNode = lRootNode->GetChild(0);

			FbxMesh* lMesh = lNode->GetMesh();

			

			if (!lMesh)
				return nullptr;

			
			std::ofstream file;

			file.open("DataFromFbxVector.txt");
			char* buffer = new char[1024];

			sprintf_s(buffer, 1024, "Number of children in Root: %d\n\n\n\n", lRootNode->GetChildCount());

			file << buffer;

			if (lMesh->IsTriangleMesh())
				file << "It's a triangle mesh!";
			else
				file << "It's NOT a triangle mesh!";

			


			FbxVector4* vertexArray = lMesh->GetControlPoints();

			for (int i = 0; i < lMesh->GetControlPointsCount(); i++)
			{
				sprintf(buffer, "(%f, %f, %f)\n", vertexArray[i].mData[0], vertexArray[i].mData[1], vertexArray[i].mData[2]);
				file << buffer;
			}

			delete buffer;

			file.close();

			Polygon* polygons = new Polygon[lMesh->GetPolygonCount()];

			int polygonCount = lMesh->GetPolygonCount();

			int index = 0;

			buffer = new char[1024];
			file.open("DataFromPolygons.txt");

			for (int i = 0; i < lMesh->GetPolygonCount(); i++)
			{
				

				index = lMesh->GetPolygonVertex(i, 0);

				sprintf(buffer, "\n\nPolygon #%d\nPolygon Vertex Index #%d: ", i, index);
				file << buffer;

				polygons[i].vertex1.x = (float)vertexArray[index].mData[0];
				polygons[i].vertex1.y = (float)vertexArray[index].mData[1];
				polygons[i].vertex1.z = (float)vertexArray[index].mData[2];

				sprintf(buffer, "(%f, %f, %f)\n", polygons[i].vertex1.x, polygons[i].vertex1.y, polygons[i].vertex1.z);
				file << buffer;


				
				index = lMesh->GetPolygonVertex(i, 1);
				sprintf(buffer, "Polygon Vertex Index #%d: ", index);
				file << buffer;

				polygons[i].vertex2.x = (float)vertexArray[index].mData[0];
				polygons[i].vertex2.y = (float)vertexArray[index].mData[1];
				polygons[i].vertex2.z = (float)vertexArray[index].mData[2];
				sprintf(buffer, "(%f, %f, %f)\n", polygons[i].vertex2.x, polygons[i].vertex2.y, polygons[i].vertex2.z);
				file << buffer;
				
				index = lMesh->GetPolygonVertex(i, 2);
				sprintf(buffer, "Polygon Vertex Index #%d: ", index);
				file << buffer;

				polygons[i].vertex3.x = (float)vertexArray[index].mData[0];
				polygons[i].vertex3.y = (float)vertexArray[index].mData[1];
				polygons[i].vertex3.z = (float)vertexArray[index].mData[2];
				sprintf(buffer, "(%f, %f, %f)\n", polygons[i].vertex3.x, polygons[i].vertex3.y, polygons[i].vertex3.z);
				file << buffer;
			}

			file.close();
			delete buffer;


			model = new Model();
			model->Vertices = new DirectX::XMFLOAT3[polygonCount * 3];
			model->NumVertices = polygonCount * 3;

			file.open("DataFromPolygonToModelTransfer.txt");
			buffer = new char[1024];

			for (int i = 0; i < lMesh->GetPolygonCount() * 3; i += 3)
			{
				model->Vertices[i] = polygons[i/3].vertex1;
				model->Vertices[i+1] = polygons[i/3].vertex2;
				model->Vertices[i+2] = polygons[i/3].vertex3;
			
				sprintf_s(buffer, 1024, "Polygon #%d:\n(%f, %f, %f)\n(%f, %f, %f)\n(%f, %f, %f)\n\n"
					, i / 3
					, model->Vertices[i].x, model->Vertices[i].y, model->Vertices[i].z
					, model->Vertices[i+1].x, model->Vertices[i+1].y, model->Vertices[i+1].z
					, model->Vertices[i+2].x, model->Vertices[i+2].y, model->Vertices[i+2].z);
			
				file << buffer;
			}

			delete buffer;
			file.close();


			// delete [] polygons;
		}
		else
			return nullptr;
	}
	else
		return nullptr;



	return model;
}