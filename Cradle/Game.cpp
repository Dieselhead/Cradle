#include "Game.h"
#include <d3d11.h>
#include "Timer.h"
#include "FbxModelLoader.h"

#include "SolidColorPS.h"
#include "SolidColorVS.h"

#include <fstream>

#define clamp(value, lb, ub) max(lb, min(ub, value))


Game::Game()
{
	
	m_windowType = WindowedBorderless;
	temp = false;

	m_timer = new Timer();

	m_d3dContext = 0;
	m_d3dDevice = 0;
	m_swapChain = 0;
	m_backbufferTarget = 0;
	m_modelLoader = 0;
	m_testModel = 0;
	m_pixelShader = 0;
	m_vertexShader = 0;
	m_vertexBuffer = 0;
	m_inputLayout = 0;

	m_camera = 0;
}

Game::~Game()
{
	Shutdown();
}

void Game::Resize(unsigned int width, unsigned int height)
{
	// Resize window
	// IF WindwoType != FullScreen
	SetWindowPos(m_hWnd, (HWND)0, 0, 0, width, height, 0);


	if (m_swapChain)
	{
		m_d3dContext->OMSetRenderTargets(0, 0, 0);

		m_backbufferTarget->Release();

		HRESULT hr;

		// Save the current settings for revert

		hr = m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		// error handling

		ID3D11Texture2D* pBuffer;
		hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D)
						, (LPVOID*)&pBuffer);

		// Error handling

		hr = m_d3dDevice->CreateRenderTargetView(pBuffer, NULL, &m_backbufferTarget);

		// Error handling
		pBuffer->Release();

		m_d3dContext->OMSetRenderTargets(1, &m_backbufferTarget, NULL);

		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;

		m_d3dContext->RSSetViewports(1, &vp);

	}
	else
	{
		// Create swap chain?
	}
}

LRESULT CALLBACK Game::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hDc;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		hDc = BeginPaint(hWnd, &paintStruct);
		EndPaint(hWnd, &paintStruct);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

bool Game::Initialize(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	InitializeWindow(hInstance, prevInstance, cmdLine, cmdShow);
	InitializeDirectX();

	LoadContent();
	

	return true;
}

bool Game::InitializeWindow(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	// Load settings
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL,	IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "GameWindowClass";
	
	wndClass.lpfnWndProc = Game::WndProc;
	
	if (!RegisterClassEx(&wndClass))
		return false;

	// Check settings for size
	// Position in center of screen?
	RECT rc = { 0, 0, 1280, 720 };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	HWND hwnd = CreateWindow("GameWindowClass", "GameWindow", WS_OVERLAPPEDWINDOW
			, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top
			, NULL, NULL, hInstance, NULL);
		

	if (!hwnd)
		return false;

	ShowWindow(hwnd, cmdShow);

	m_hWnd = hwnd;
	m_hInstance = hInstance;

	return true;

}

void Game::Shutdown()
{
	UnloadContent();

	if (m_d3dDevice) m_d3dDevice->Release();
	if (m_d3dContext) m_d3dContext->Release();
	if (m_swapChain) m_swapChain->Release();
	if (m_backbufferTarget) m_backbufferTarget->Release();
	if (m_timer) delete m_timer;


	m_d3dDevice = 0;
	m_d3dContext = 0;
	m_swapChain = 0;
	m_backbufferTarget = 0;
	m_timer = 0;

	if (m_modelLoader)
		delete m_modelLoader;

	m_modelLoader = 0;

//	if (m_testModel)
//		delete m_testModel;

	if (m_cbPerObjectBuffer) m_cbPerObjectBuffer->Release();

	if (m_pixelShader) m_pixelShader->Release();
	if (m_vertexShader) m_vertexShader->Release();
	if (m_vertexBuffer) m_vertexBuffer->Release();
	if (m_inputLayout) m_inputLayout->Release();

	if (m_camera)
		delete m_camera;

	m_camera = 0;

	if (m_rsWireFrame) m_rsWireFrame->Release();
	m_rsWireFrame = 0;
}

void Game::LoadContent()
{
	
	m_modelLoader = new FbxModelLoader();

	int error = m_modelLoader->Initialize();
	if (error < 1)
	{
		char* string = "";

		switch (error)
		{
		case -1: string ="-1";
			break;
		case -2:
			string = "-2";
			break;
		case -3:
			string = "-3";
			break;
		}
		MessageBoxA(m_hWnd, string, "Fail", MB_OK);
		exit(-1);

		delete string;
	}

	// m_testModel = nullptr;

	m_testModel = m_modelLoader->LoadModel("t80_turret.fbx");


	if (!m_testModel)
		exit(-1);

	HRESULT result;

	result = m_d3dDevice->CreateVertexShader(g_VSMain, sizeof(g_VSMain), nullptr, &m_vertexShader);
	if (FAILED(result))
		exit(-1);

	result = m_d3dDevice->CreatePixelShader(g_PSMain, sizeof(g_PSMain), nullptr, &m_pixelShader);
	if (FAILED(result))
		exit(-1);

	D3D11_INPUT_ELEMENT_DESC VertexPosLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	unsigned int totalLayoutElements = ARRAYSIZE(VertexPosLayout);


	/*
	VertexPos verts[] = 
	{
		DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f)
		, DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f)
		, DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f)
	};
	*/

	char* buffer = new char[256];

	// File f;
	// f.Open("debug.txt");

	std::ofstream file;
	file.open("debug.txt");

	DirectX::XMFLOAT4 v;

	VertexPos* verts = new VertexPos[m_testModel->NumVertices];
	for (unsigned int i = 0; i < m_testModel->NumVertices; i++)
	{
		v.x = m_testModel->Vertices[i].x;
		v.y = m_testModel->Vertices[i].y;
		v.z = m_testModel->Vertices[i].z;
		v.w = 1.0f;

		// verts[i].Position = m_testModel->Vertices[i];
		verts[i].Position = v;
		

		sprintf(buffer, "(%f, %f, %f)\n"
			,verts[i].Position.x
			,verts[i].Position.y
			,verts[i].Position.z);
		/*
			, clamp(verts[i].Position.x, -1.0f, 1.0f)
			, clamp(verts[i].Position.y, -1.0f, 1.0f)
			, clamp(verts[i].Position.z, -1.0f, 1.0f));
		// */

		file << buffer;

		// MessageBox(m_hWnd, buffer, "boop", MB_OK);
		// f.Write(buffer);


	}

	delete buffer;
	file.close();

	// f.Close();

	result = m_d3dDevice->CreateInputLayout(VertexPosLayout, totalLayoutElements, g_VSMain, sizeof(g_VSMain), &m_inputLayout);
	if (FAILED(result))
		exit(-1);

	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));

	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// vertexDesc.ByteWidth = sizeof(vertexDesc) * m_testModel->NumVertices;
	vertexDesc.ByteWidth = sizeof(VertexPos) * m_testModel->NumVertices;

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	// resourceData.pSysMem = m_testModel->Vertices;
	resourceData.pSysMem = verts;


	result = m_d3dDevice->CreateBuffer(&vertexDesc, &resourceData, &m_vertexBuffer);
	if (FAILED(result))
		exit(-1);


	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	result = m_d3dDevice->CreateBuffer(&cbbd, nullptr, &m_cbPerObjectBuffer);

	if (FAILED(result))
		exit(-1);


	m_camera = new Camera();
	m_camera->Initialize(3.14f * 0.4f, 1280.0f/720.0f, 0.1f, 100.0f);


	D3D11_RASTERIZER_DESC rsd;
	ZeroMemory(&rsd, sizeof(D3D11_RASTERIZER_DESC));
	// rsd.FillMode = D3D11_FILL_WIREFRAME;
	rsd.FillMode = D3D11_FILL_SOLID;
	// rsd.CullMode = D3D11_CULL_NONE;
	// rsd.CullMode = D3D11_CULL_FRONT;
	rsd.CullMode = D3D11_CULL_NONE;
	rsd.ScissorEnable = false;


	result = m_d3dDevice->CreateRasterizerState(&rsd, &m_rsWireFrame);
	if (FAILED(result))
		exit(-1);

	m_d3dContext->RSSetState(m_rsWireFrame);

}

void Game::UnloadContent()
{
	
}

void Game::Update(double total, double delta)
{
	m_camera->Update();
}

void Game::Update()
{
	if (m_timer)
		m_timer->Update();

	Update(m_timer->GetTotal(), m_timer->GetDelta());
	Render();

	if (GetAsyncKeyState(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	if (GetAsyncKeyState(0x4F))
		PostQuitMessage(0);

	if (GetAsyncKeyState(VK_LEFT))
	{
		
		m_camera->m_position.x += (float)m_timer->GetDelta() * -100.0f;
		
	}
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		m_camera->m_position.x += (float)m_timer->GetDelta() * 100.0f;


	}

	if (GetAsyncKeyState(VK_UP))
	{
		m_camera->m_position.z += (float)m_timer->GetDelta() * -100.0f;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		m_camera->m_position.z += (float)m_timer->GetDelta() * 100.0f;

	}

	if (GetAsyncKeyState(VK_SPACE))
	{
			m_camera->m_position.y += (float)m_timer->GetDelta() * 100.0f;

	}
	else if (GetAsyncKeyState(VK_RETURN))
	{
			m_camera->m_position.y += (float)m_timer->GetDelta() * -100.0f;

	}
}

void Game::Start()
{
	MessageLoop();
}

void Game::MessageLoop()
{
	MSG msg = { 0 };

	while (true)
	// while (msg.message != WM_QUIT)
	{
		if (msg.message == WM_QUIT)
			break;

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
		}
	}

	Shutdown();
}

void Game::Render()
{
	if (!m_d3dContext)
	{
		return;
	}

	float clearColor[4] = { 0.25f, 0.25f, 1.0f, 1.0f };
	m_d3dContext->ClearRenderTargetView(m_backbufferTarget, clearColor);

	/*
	ID3D11RasterizerState* rs;

	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));

	rsDesc.CullMode = D3D11_CULL_NONE;

	m_d3dDevice->CreateRasterizerState(&rsDesc, &rs);

	m_d3dContext->RSSetState(rs);
	*/
	cbPerObj.WPV = DirectX::XMMatrixTranspose( m_camera->m_wvp);


	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;

	m_d3dContext->IASetInputLayout(m_inputLayout);
	m_d3dContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_d3dContext->PSSetShader(m_pixelShader, 0, 0);
	m_d3dContext->VSSetShader(m_vertexShader, 0, 0);
	
	m_d3dContext->UpdateSubresource(m_cbPerObjectBuffer, 0, nullptr, &cbPerObj, 0, 0);

	m_d3dContext->VSSetConstantBuffers(0, 1, &m_cbPerObjectBuffer);

	m_d3dContext->Draw(m_testModel->NumVertices, 0);


	m_swapChain->Present(0, 0);
}

bool Game::InitializeDirectX()
{
	RECT dimensions;
	GetClientRect(m_hWnd, &dimensions);

	m_windowWidth = dimensions.right - dimensions.left;
	m_windowHeight = dimensions.bottom - dimensions.top;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE
		, D3D_DRIVER_TYPE_WARP
		, D3D_DRIVER_TYPE_SOFTWARE
	};

	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_0
		, D3D_FEATURE_LEVEL_10_1
		, D3D_FEATURE_LEVEL_10_0
	};

	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Load settings for these values
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = m_windowWidth;
	swapChainDesc.BufferDesc.Height = m_windowHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = m_hWnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	

	unsigned int creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result;
	unsigned int driver = 0;

	for (driver = 0; driver < totalDriverTypes; ++driver)
	{
		result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver]
		, 0, creationFlags, featureLevels, totalFeatureLevels
		, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain
		, &m_d3dDevice, &m_featureLevel, &m_d3dContext);

		if (SUCCEEDED(result))
		{
			m_driverType = driverTypes[driver];
			break;
		}
	}

	if (FAILED(result))
	{
		// Failed to create the D3D Device
		return false;
	}


	ID3D11Texture2D* backBufferTexture;

	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D)
		, (LPVOID*)&backBufferTexture);

	if (FAILED(result))
	{
		return false;
	}

	result = m_d3dDevice->CreateRenderTargetView(backBufferTexture, 0
		, &m_backbufferTarget);

	if (backBufferTexture)
		backBufferTexture->Release();
	
	if (FAILED(result))
	{
		// Failed to create render target view
		return false;
	}

	m_d3dContext->OMSetRenderTargets(1, &m_backbufferTarget, 0);

	D3D11_VIEWPORT viewPort;
	viewPort.Width = static_cast<float>(m_windowWidth);
	viewPort.Height = static_cast<float>(m_windowHeight);
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;

	m_d3dContext->RSSetViewports(1, &viewPort);

	return true;
}