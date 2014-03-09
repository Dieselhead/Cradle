#ifndef _CRADLEGAME_
#define _CRADLEGAME_

#include <stdio.h>
#define HRPRINT(hr, failed, success) wprintf((FAILED(hr)) ? failed : success);
#define ReleaseCOM(object) if (object) object->Release(); object = nullptr;
#define DeletePtr(ptr) if (ptr) delete ptr; ptr = nullptr;
#define DeleteArray(arr) if (arr) delete [] arr; arr = nullptr;

#include <Windows.h>
#include "BasicTimer.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include "CrTexture2D.h"
#include "CradleFileManager.h"
#include "CradleVertexLayouts.h"
#include "CradleActor.h"


class CradleWindow;
class CradleD3D;
class CradleInput;
class CradleCamera;

class CradleGame
{
public:
	__declspec(dllexport) CradleGame();
	virtual ~CradleGame();

	__declspec(dllexport) int Init(CradleWindow* outputWindow);
	__declspec(dllexport)int Start();

	void SetOutputWindow(CradleWindow* newWindow);

	CradleActor* m_Actor;
	const int m_numActors = 3000;
	const wchar_t* m_meshToUse = L"monkey";

	int m_fps;
	float m_total;
	CradleCamera* m_camera;
	// ID3D11Buffer* m_vertexBuffer; 


	// ID3D11VertexShader* m_vs;
	// ID3D11PixelShader* m_ps;
	ID3D11InputLayout* m_inputLayout;

	CradleFileManager* m_Resources;

	ID3D11Buffer* m_instanceBuffer;

	CradleVertexDeclarations::InstanceData* m_instanceData;


	ID3D11Buffer* m_vertexBuffer;

	void UpdateActor(CradleActor* actors, int start, int count);

	



	// TODO
	// Model manager contains Models
		// Model contains Mesh
			// Mesh contains
				// Vertex buffer
				// Pixel shader type
				// Vertex shader type
				// Vertex structure type
				// Input layout
	// Meshes, PS, VS, vertex structures, input layouts are all handled by managers

	// On Mesh->Draw()
		// Get transform matrix for this mesh
		// Add to list of meshes to draw this frame

	CrTexture2D* m_texture;
	ID3D11SamplerState* m_samplerState;

	
	int Update();
	void Render();

	int BaseUpdate();
	void CreateD3D();
	void MessageLoop();
	
	CradleWindow* m_cWindow;
	CradleD3D* m_cd3d;
	CradleInput* m_cInput;
	BasicTimer* m_timer;
};


#endif