#include "CradleGame.h"
#include "CradleWindow.h"
#include "CradleD3D.h"
#include "CradleInput.h"
#include "CradleCamera.h"
#include "CradleVertexLayouts.h"

#include "ObjModel.h"

#pragma comment (lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

#include <stdio.h>
#include <string>

#include "CradleMesh.h"

#include <time.h>
#include <thread>



CradleGame::CradleGame()
{
	m_cWindow = nullptr;
	m_cd3d = nullptr;
	m_timer = nullptr;
	m_cInput = nullptr;
	m_Resources = nullptr;

	m_total = 0.0f;
	m_fps = 0;
	m_camera = nullptr;
	m_texture = nullptr;

	m_instanceData = nullptr;
	m_vertexBuffer = nullptr;

	m_Actor = nullptr;

	// m_vs = nullptr;
	// m_ps = nullptr;
}

CradleGame::~CradleGame()
{

	if (m_Actor) delete[] m_Actor;
	m_Actor = nullptr;

	
	if (m_cd3d) delete m_cd3d;
	m_cd3d = nullptr;

	if (m_timer) delete m_timer;
	m_timer = nullptr;

	if (m_cInput) delete m_cInput;
	m_cInput = nullptr;

	if (m_texture) delete m_texture;
	m_texture = nullptr;

	// ReleaseCOM(m_vertexBuffer);
	// ReleaseCOM(m_vs);
	// ReleaseCOM(m_ps);
	DeletePtr(m_camera);

	if (m_Resources) delete m_Resources;
	m_Resources = nullptr;

	if (m_instanceData) delete[] m_instanceData;
	m_instanceData = nullptr;

	ReleaseCOM(m_vertexBuffer);

}

void CradleGame::CreateD3D()
{
	if (m_cd3d == nullptr)
	{
		m_cd3d = new CradleD3D();

		m_cd3d->Create(m_cWindow);
	}
}

void CradleGame::SetOutputWindow(CradleWindow* newWindow)
{
	m_cWindow = newWindow;

}

int CradleGame::Init(CradleWindow* outputWindow)
{
	SetOutputWindow(outputWindow);
	
	CreateD3D();
	
	m_timer = new BasicTimer();

	m_cInput = new CradleInput(this);
	m_cInput->Initialize();

	m_Resources = new CradleFileManager(m_cd3d);

	return 0;
}


int CradleGame::Start()
{

	HRESULT hr = E_FAIL;


	wchar_t* filePath = L"D:\\Dropbox\\Dropbox\\_DirectX\\Source\\Cradle\\Debug\\Content\\Shaders\\SolidColor.hlsl";


	if (!m_Resources->LoadShaders())
	{
		Sleep(5000);
		return 0;
	}
	m_Resources->LoadMeshes();




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
	}
	, { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};


	unsigned int totalLayoutElements = ARRAYSIZE(vertexLayout);

	hr = m_cd3d->m_d3dDevice->CreateInputLayout(vertexLayout
	, totalLayoutElements
	, vsBuffer->GetBufferPointer()
	, vsBuffer->GetBufferSize()
	, &m_inputLayout);

	HRPRINT(hr, L"Failed to create InputLayout\n", L"Created InputLayout\n");

	D3D11_SAMPLER_DESC colorMapDesc;
	ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));
	colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_cd3d->m_d3dDevice->CreateSamplerState(&colorMapDesc, &m_samplerState);
	HRPRINT(hr, L"Failed to create sampler\n", L"Created sampler\n");

	*/


	m_Actor = new CradleActor[m_numActors];


	srand(time(NULL));

	for (int i = 0; i < m_numActors; i++)
	{
		int j = rand();
		double r = rand() / ((double)(RAND_MAX + 1));

		if (r > 0.75)
		{
			m_Actor[i].SetMesh((CradleMesh*)m_Resources->GetMesh(L"monkey"));
		}
		else if (r > 0.50)
		{
			m_Actor[i].SetMesh((CradleMesh*)m_Resources->GetMesh(L"monkey_smooth"));
		}
		else if (r > 0.25)
		{
			m_Actor[i].SetMesh((CradleMesh*)m_Resources->GetMesh(L"cube"));
		}
		else
		{
			m_Actor[i].SetMesh((CradleMesh*)m_Resources->GetMesh(L"cylinder"));
		}

		m_Actor[i].m_Offset.x = (((float)i - ((float)m_numActors * 0.5f))* 5.0f) / 50.0f;
		m_Actor[i].m_Offset.y = (i % 50) * 5.0f;
		// wprintf_s(L"RANDOM: float(%f) int(%d)\n", r, j);
		m_Actor[i].m_rotationRate =  (float)((float)r * 1.0f + 1.0f);
	}

	

	m_camera = new CradleCamera(this);
	m_camera->Initialize(60.0f * 3.14f / 180, (float)((float)m_cWindow->GetClientWidth() / (float)m_cWindow->GetClientHeight()), 0.1f, 1000.0f);
	wprintf_s(L"Created camera with (%d, %d)\n", m_cWindow->GetClientWidth(), m_cWindow->GetClientHeight());







	unsigned int stride = sizeof(CradleVertexDeclarations::VertexPosTex);
	unsigned int offset = 0;

	ID3D11InputLayout* il = m_Resources->GetInputLayout(L"SolidColor");
	CradleMesh* mesh = (CradleMesh*)m_Resources->GetMesh(m_meshToUse);
	ID3D11Buffer* vb = ((CradleMesh*)m_Resources->GetMesh(m_meshToUse))->GetVertexBuffer();

	m_cd3d->m_d3dContext->IASetInputLayout(il);
	m_cd3d->m_d3dContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	m_cd3d->m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	ID3D11RasterizerState* rState;
	CD3D11_RASTERIZER_DESC rDesc;


	rDesc.AntialiasedLineEnable = false;
	rDesc.CullMode = D3D11_CULL_BACK;
	rDesc.DepthBias = 0;
	rDesc.DepthBiasClamp = 0.0f;
	rDesc.DepthClipEnable = true;
	rDesc.FillMode = D3D11_FILL_SOLID;
	rDesc.FrontCounterClockwise = false;
	rDesc.MultisampleEnable = false;
	rDesc.ScissorEnable = false;
	rDesc.SlopeScaledDepthBias = 0.0f;

	m_cd3d->m_d3dDevice->CreateRasterizerState(&rDesc, &rState);

	m_cd3d->m_d3dContext->RSSetState(rState);




	m_cd3d->m_d3dContext->VSSetShader(m_Resources->GetVertexShader(L"SolidColor"), 0, 0);
	m_cd3d->m_d3dContext->PSSetShader(m_Resources->GetPixelShader(L"SolidColor"), 0, 0);
	// m_cd3d->m_d3dContext->PSSetShaderResources(0, 1, &m_texture->m_colorData);
	// m_cd3d->m_d3dContext->PSSetSamplers(0, 1, &m_samplerState);
	// */




	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA  instanceData;

	m_instanceData = new CradleVertexDeclarations::InstanceData[m_numActors];




	for (int i = 0; i < m_numActors; i++)
	{

		m_instanceData[i].world0 = DirectX::XMFLOAT4(
			m_Actor[i].m_mWorld.m[0][0],
			m_Actor[i].m_mWorld.m[0][1],
			m_Actor[i].m_mWorld.m[0][2],
			m_Actor[i].m_mWorld.m[0][3]
			);

		m_instanceData[i].world1 = DirectX::XMFLOAT4(
			m_Actor[i].m_mWorld.m[1][0],
			m_Actor[i].m_mWorld.m[1][1],
			m_Actor[i].m_mWorld.m[1][2],
			m_Actor[i].m_mWorld.m[1][3]
			);

		m_instanceData[i].world2 = DirectX::XMFLOAT4(
			m_Actor[i].m_mWorld.m[2][0],
			m_Actor[i].m_mWorld.m[2][1],
			m_Actor[i].m_mWorld.m[2][2],
			m_Actor[i].m_mWorld.m[2][3]
			);

		m_instanceData[i].world3 = DirectX::XMFLOAT4(
			m_Actor[i].m_mWorld.m[3][0],
			m_Actor[i].m_mWorld.m[3][1],
			m_Actor[i].m_mWorld.m[3][2],
			m_Actor[i].m_mWorld.m[3][3]
			);
	}


	instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	instanceBufferDesc.ByteWidth = sizeof(CradleVertexDeclarations::InstanceData) * m_numActors;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;



	instanceData.pSysMem = m_instanceData;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;


	// Create the instance buffer.
	// HRESULT result = m_cd3d->m_d3dDevice->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);

	// HRPRINT(result, L"FAILED TO CREATE INSTANCE BUFFER\n", L"CREATED INSTANCE\n");







	Update();
	


	/*
	ID3D11Buffer* vb_ = ((CradleMesh*)m_Resources->GetMesh(m_meshToUse))->GetVertexBuffer();
	ID3D11Buffer* indexBuffer = ((CradleMesh*)m_Resources->GetMesh(m_meshToUse))->GetIndexBuffer();
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
	*/







	// Enter game loop
	MessageLoop();

	return 0;
}

void CradleGame::MessageLoop()
{
	// Calculate delta time here and in the loop
	
	do
	{
		this->BaseUpdate();
	}
	while (m_cWindow->MessageLoop());
}

void DrawActor(CradleActor* actors, int start, int count)
{
	for (int i = start; i < start + count; i++)
	{
		actors[i].Draw();
	}
}

void CradleGame::Render()
{

	float clearColor[4] = { 0.20f, 0.0f, 0.25f, 1.0f };
	float f = 0.0f;

	m_cd3d->Clear(clearColor);



	
	f = m_timer->UpdateFoo();
	for (int i = 0; i < m_numActors; i++)
	{
		m_Actor[i].Draw();
	}
	wprintf_s(L"Actors.Draw(): %f\n", (m_timer->UpdateFoo() - f));
	









	f = m_timer->UpdateFoo();
	m_Resources->Draw();
	wprintf_s(L"FileManager.Draw(): %f\n", (m_timer->UpdateFoo() - f));


	/*
	unsigned int stride = sizeof(CradleVertexDeclarations::VertexPosTex);
	unsigned int offset = 0;


	
	ID3D11Buffer* vb = ((CradleMesh*)m_Resources->GetMesh(m_meshToUse))->GetVertexBuffer();

	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	strides[0] = sizeof(CradleVertexDeclarations::VertexPosTex);
	strides[1] = sizeof(CradleVertexDeclarations::InstanceData);

	offsets[0] = 0;
	offsets[1] = 0;

	bufferPointers[0] = vb;
	bufferPointers[1] = m_instanceBuffer;
	


	m_cd3d->m_d3dContext->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);
	*/



	// DRAW INDEXED INSTANCED
	int num = ((CradleMesh*)m_Resources->GetMesh(m_meshToUse))->GetNumIndices();
	CradleMesh* mesh = (CradleMesh*)m_Resources->GetMesh(m_meshToUse);


	// m_cd3d->m_d3dContext->DrawIndexedInstanced(num, m_numActors, 0, 0, 0);



	m_cd3d->m_swapChain->Present(0, 0);

}


void CradleGame::UpdateActor(CradleActor* actors, int start, int count)
{
	for (int i = start; i < start + count; i++)
	{
		actors[i].Draw();
	}
}

void UpdateActor2(CradleActor* actor, int start, int count, BasicTimer* timer)
{
	for (int i = start; i < start + count; i++)
	{
		actor[i].Update(timer);
	}
}


int CradleGame::Update()
{
	m_camera->Update();

	static float lasttime = 0.0f;

	float f = m_timer->UpdateFoo();


	std::thread t1(UpdateActor2, m_Actor, 0, 500, m_timer);
	std::thread t2(UpdateActor2, m_Actor, 500, 500, m_timer);
	std::thread t3(UpdateActor2, m_Actor, 1000, 500, m_timer);
	std::thread t4(UpdateActor2, m_Actor, 1500, 500, m_timer);
	std::thread t5(UpdateActor2, m_Actor, 2000, 500, m_timer);
	std::thread t6(UpdateActor2, m_Actor, 2500, 500, m_timer);


	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();

	wprintf_s(L"Actors.Update: %f\n", m_timer->UpdateFoo() - f);

	/*
	for (unsigned short i = 0; i < m_numActors; i++)
	{
		m_Actor[i].Update(m_timer);
	}
	*/



	
	/* */
	



	// STARTING HERE
	
	static char UPDATUS = 0;
	/*
	if (UPDATUS == 0 || true)
	{
		UPDATUS++;


		for (unsigned short i = 0; i < m_numActors; i++)
		{
			m_instanceData[i].world0 = DirectX::XMFLOAT4(m_Actor[i].m_mWorld.m[0]);
			m_instanceData[i].world1 = DirectX::XMFLOAT4(m_Actor[i].m_mWorld.m[1]);
			m_instanceData[i].world2 = DirectX::XMFLOAT4(m_Actor[i].m_mWorld.m[2]);
			m_instanceData[i].world3 = DirectX::XMFLOAT4(m_Actor[i].m_mWorld.m[3]);
		}
	}

	*/

	
	// END HERE
	/*
	D3D11_MAPPED_SUBRESOURCE ms;
	m_cd3d->m_d3dContext->Map(m_instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	memcpy(ms.pData, m_instanceData, sizeof(CradleVertexDeclarations::InstanceData) * m_numActors);
	m_cd3d->m_d3dContext->Unmap(m_instanceBuffer, 0);
	*/





	if (m_cInput->KeyPressed(DIK_ESCAPE))
		PostQuitMessage(0);

	return 0;
}

int CradleGame::BaseUpdate()
{
	m_timer->Update();

	if (m_cInput)
		m_cInput->Update();

	m_total += m_timer->GetDelta();
	m_fps++;

	if (m_total > 1.0f)
	{
		m_total -= 1.0f;
		wprintf_s(L"FPS: %d\n", m_fps);
	
		m_fps = 0;
	}


	// PrePhysicsUpdate();
	Update();
	// PostPhysicsUpdate();

	Render();

	return 0;
}