#include "CrTexture2D.h"

CrTexture2D::CrTexture2D(HWND hWnd, COMDLG_FILTERSPEC* fileFilter, LPWSTR filePath, ID3D11Device* d3dDevice)
	: CradleFile(hWnd, fileFilter, filePath)
{
	m_d3dDevice = d3dDevice;
	m_colorData = nullptr;
}

CrTexture2D::~CrTexture2D()
{
	if (m_colorData) m_colorData->Release();

	m_colorData = nullptr;
}

HRESULT CrTexture2D::LoadFile()
{
	HRESULT hr = E_FAIL;
	
	if (SUCCEEDED(hr))
	{
		hr = LoadFileDDS();
	}

	return hr;
}

HRESULT CrTexture2D::LoadFileDDS()
{
	HRESULT hr = E_FAIL;
	
	/*
	// Check if file is already holding data
	if (m_filePath != nullptr)
	{
		hr = DirectX::CreateDDSTextureFromFile(m_d3dDevice, m_filePath, nullptr, &m_colorData);
		HRPRINT(hr, L"Failed to create DDS", L"Created DDS");
		if (SUCCEEDED(hr))
		{
			// this is now holding a texture
		}
		else
		{
			// Failed to load texture
		}
	}
	*/

	return hr;
}