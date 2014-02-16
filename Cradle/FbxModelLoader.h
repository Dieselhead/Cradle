#pragma once
#include <fbxsdk.h>
#include <DirectXMath.h>
#include "FbxModel.h"

struct Vector3
{
	float x;
	float y;
	float z;
};

class FbxModelLoader
{
public:
	FbxModelLoader();
	~FbxModelLoader();

	int Initialize();

	Model* LoadModel(const char* fileName);

private:
	FbxManager* m_manager;
	FbxImporter* m_importer;
	



};