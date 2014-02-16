#include "CradleD3D.h"
#include "CradleWindow.h"
#include "CradleGame.h"

#include <stdio.h>



CradleD3D::CradleD3D()
	: m_backBufferRenderTarget(0), m_d3dContext(0)
	, m_d3dDevice(0), m_swapChain(0)
	, m_driverType(D3D_DRIVER_TYPE_NULL)
	, m_featureLevel(D3D_FEATURE_LEVEL_11_0)
	, m_depthStencilBuffer(0)
	, m_depthStencilView(0)
{
	
}

CradleD3D::~CradleD3D()
{

	if (m_d3dContext) m_d3dContext->Release();
	if (m_d3dDevice) m_d3dDevice->Release();
	if (m_swapChain) m_swapChain->Release();
	if (m_backBufferRenderTarget) m_backBufferRenderTarget->Release();
	
	ReleaseCOM(m_depthStencilBuffer);
	if (m_depthStencilView) m_depthStencilView->Release();



	m_d3dContext = nullptr;
	m_d3dDevice = nullptr;
	m_swapChain = nullptr;
	m_backBufferRenderTarget = nullptr;
	m_depthStencilView = nullptr;
}

void CradleD3D::CreateD3D(CradleWindow* window)
{
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

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width = window->GetClientWidth();
	swapChainDesc.BufferDesc.Height = window->GetClientHeight();
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window->GetWindowHandle();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;

	unsigned int creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result;
	unsigned int driver = 0;

	for (driver = 0; driver < totalDriverTypes; driver++)
	{
		result = D3D11CreateDeviceAndSwapChain(
			0
			, driverTypes[driver]
			, 0
			, creationFlags
			, featureLevels
			, totalFeatureLevels
			, D3D11_SDK_VERSION
			, &swapChainDesc
			, &m_swapChain
			, &m_d3dDevice
			, &m_featureLevel
			, &m_d3dContext);

		if (SUCCEEDED(result))
		{
			m_driverType = driverTypes[driver];
			wprintf(L"Created D3D\n");
			break;
		}
	}

	if (FAILED(result))
	{
		// TODO
		// Shutdown application
		wprintf(L"Failed to create D3D\n");
		// PostQuitMessage(0);
	}
}

void CradleD3D::Create(CradleWindow* window)
{
	/*
	RECT dimensions;
	GetClientRect(window->GetWindowHandle(), &dimensions);

	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	
	printf("m_width: %d\nm_height: %d\n", window->GetWidth(), window->GetHeight());
	printf("width: %d\nheight: %d\n", width, height);


	width = window->GetWidth();
	height = window->GetHeight();



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

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window->GetWindowHandle();
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	

	unsigned int creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result;
	unsigned int driver = 0;

	for (driver = 0; driver < totalDriverTypes; driver++)
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
		// TODO
		// ABORT WITH ERROR

		// return;
	}

	*/

	CreateD3D(window);

	Resize(window->GetClientWidth(), window->GetClientHeight());

	/*
	ID3D11Texture2D* backBufferTexture;

	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D)
					, (LPVOID*)&backBufferTexture);

	if (FAILED(result))
	{
		// TODO
		// ABORT WITH ERROR

		float f = 4.0f;
	}

	result = m_d3dDevice->CreateRenderTargetView(backBufferTexture, 0
				, &m_backBufferRenderTarget);

	if (backBufferTexture)
		backBufferTexture->Release();

	if (FAILED(result))
	{
		// TODO
		// ABORT WITH ERROR
	}



	m_d3dContext->OMSetRenderTargets(1, &m_backBufferRenderTarget, 0);

	
	m_viewport.Width = static_cast<float>(width);
	m_viewport.Height = static_cast<float>(height);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	m_d3dContext->RSSetViewports(1, &m_viewport);
	*/
}

void CradleD3D::Resize(UINT width, UINT height)
{
	// Release existing render targets & depth stencil
	// Resize window
	// Resize swapchain
	// Create new render targets
	// Resize and set new viewport

	

	DXGI_SWAP_CHAIN_DESC debugDesc;
	m_swapChain->GetDesc(&debugDesc);

	wprintf(L"Before D3D Resize:\n");
	wprintf(L"Width: %d, Height: %d\n", debugDesc.BufferDesc.Width, debugDesc.BufferDesc.Height);
	
	HRESULT result;

	if (m_backBufferRenderTarget) m_backBufferRenderTarget->Release();
	if (m_depthStencilView) m_depthStencilView->Release();

	if (!m_swapChain)
	{
		// TODO
		// Exit application
		wprintf(L"Swapchain not created\n");
	}

	DXGI_MODE_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.RefreshRate.Denominator = 1;
	desc.RefreshRate.Numerator = 60;


	result = m_swapChain->ResizeTarget(&desc);

	if (FAILED(result))
	{
		// TODO
		// Exit application

		wprintf(L"Failed to resize target\n");
	}


	unsigned int swapChainFlags = 0;
	

#ifdef _DEBUG
	swapChainFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	result = m_swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, swapChainFlags);

	if (FAILED(result))
	{
		// TODO
		// Exit application
		wprintf(L"Failed to resize buffers\n");
	}

	ID3D11Texture2D* backBufferTexture;
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);

	if (FAILED(result))
	{
		// TODO
		// Exit application
		wprintf(L"Failed to get buffer from swapchain\n");
	}

	result = m_d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &m_backBufferRenderTarget);

	if (backBufferTexture)
		backBufferTexture->Release();

	if (FAILED(result))
	{
		// TODO
		// Exit application
		wprintf(L"Failed to create render target view\n");
		
	}

	
	D3D11_TEXTURE2D_DESC depthDesc;

	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.SampleDesc.Count = 1;		// Must match
	depthDesc.SampleDesc.Quality = 0;	// swapchain settings
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;

	result = m_d3dDevice->CreateTexture2D(&depthDesc, 0, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		// TODO
		// Exit application
		wprintf(L"Failed to create Depth Stencil texture\n");
	}

	result = m_d3dDevice->CreateDepthStencilView(m_depthStencilBuffer, 0, &m_depthStencilView);
	HRPRINT(result, L"Failed to create DepthStencilView\n", L"DepthStencilViewCreated\n");

	m_d3dContext->OMSetRenderTargets(1, &m_backBufferRenderTarget, m_depthStencilView);




	m_viewport.Width = static_cast<float>(width);
	m_viewport.Height = static_cast<float>(height);
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

	m_d3dContext->RSSetViewports(1, &m_viewport);



	m_swapChain->GetDesc(&debugDesc);
	wprintf(L"After resize:\n");
	wprintf(L"Width: %d, Height: %d\n", debugDesc.BufferDesc.Width, debugDesc.BufferDesc.Height);

}

void CradleD3D::Clear(const float* color)
{
	m_d3dContext->ClearRenderTargetView(m_backBufferRenderTarget, color);
	m_d3dContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// TODO
	// REMOVE PRESENT
}

void CradleD3D::SetFullscreen(BOOL enableFullscreen)
{
	BOOL isFullscreen = 0;

	m_swapChain->GetFullscreenState(&isFullscreen, 0);

	if (isFullscreen == enableFullscreen)
		return;

	if (enableFullscreen)
	{
		// m_game->m_cWindow->ChangeWindowType(CradleWindow::WindowTypeFullscreen);
		m_swapChain->SetFullscreenState((BOOL)enableFullscreen, 0);
	}
	else
	{
		// m_game->m_cWindow->ChangeWindowType(CradleWindow::WindowTypeWindowed);
		m_swapChain->SetFullscreenState((BOOL)enableFullscreen, 0);
	}
}

HRESULT CradleD3D::CompileShader(LPCWSTR sourceFile, LPCSTR entryPoint, LPCSTR profile, ID3DBlob** blob)
{
	if (!sourceFile || !entryPoint || !profile || !blob)
		return E_INVALIDARG;

	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", "1"
		, NULL, NULL
	};

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(sourceFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, entryPoint, profile, flags, 0, &shaderBlob, &errorBlob);

	HRPRINT(hr, L"Failed to compile shader\n", L"Compiled shader!\n");

	if (FAILED(hr))
	{
		if (errorBlob)
		{
			//char* str = (char*)errorBlob->GetBufferPointer();
			//wchar_t* wstr = new wchar_t[errorBlob->GetBufferSize()];
			//mbtowc(wstr, str, errorBlob->GetBufferSize());

			//wprintf_s(wstr);
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
			Sleep(5000);

			// delete[] wstr;
		}

		if (shaderBlob)
			shaderBlob->Release();

		return hr;
	}

	*blob = shaderBlob;

	return hr;
}