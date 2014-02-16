#ifndef _CRADLEVERTEXLAYOUT_
#define _CRADLEVERTEXLAYOUT_

#include <DirectXMath.h>
#include <d3d11.h>


namespace CradleVertexDeclarations
{

	struct VertexPosTex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		// DirectX::XMFLOAT2 tex0;

		/*
		VertexPosTex()
		{

		}

		VertexPosTex(DirectX::XMFLOAT3 p /*, DirectX::XMFLOAT2 t )
		{
			pos = p;
			// tex0 = t;
		}
		*/
	};

	struct InstanceData
	{
		DirectX::XMFLOAT4 world0;
		DirectX::XMFLOAT4 world1;
		DirectX::XMFLOAT4 world2;
		DirectX::XMFLOAT4 world3;
	};
	

	const D3D11_INPUT_ELEMENT_DESC VertexPositionTexture[] =
	{
		{
			"POSITION"						// Semantic name
			, 0								// Semantic index
			, DXGI_FORMAT_R32G32B32_FLOAT	// Format
			, 0								// Input slot
			, 0								// Aligned byte offset
			, D3D11_INPUT_PER_VERTEX_DATA	// Input slot class
			, 0								// Instance data step rate
		},
		{
			"NORMAL"						// Semantic name
			, 0								// Semantic index
			, DXGI_FORMAT_R32G32B32_FLOAT	// Format
			, 0								// Input slot
			, 12							// Aligned byte offset
			, D3D11_INPUT_PER_VERTEX_DATA	// Input slot class
			, 0								// Instance data step rate
		},
		{
			"OBJ",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			1,
			0,
			D3D11_INPUT_PER_INSTANCE_DATA,
			1
		},
		{
			"OBJ",
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			1,
			16,
			D3D11_INPUT_PER_INSTANCE_DATA,
			1
		},
		{
			"OBJ",
			2,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			1,
			32,
			D3D11_INPUT_PER_INSTANCE_DATA,
			1
		},
		{
			"OBJ",
			3,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			1,
			48,
			D3D11_INPUT_PER_INSTANCE_DATA,
			1
		}
		/*
		, { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{
			"TEXCOORD",
			1,
			DXGI_FORMAT_R32G32B32_FLOAT,
			1,
			0,
			D3D11_INPUT_PER_INSTANCE_DATA,
			1
		}*/
	};


	const unsigned int VertexPositionTextureNumElements = ARRAYSIZE(VertexPositionTexture);

}

#endif