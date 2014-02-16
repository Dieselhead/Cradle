#ifndef _CRADLED3D_
#define _CRADLED3D_

#include <d3d11.h>
#pragma comment (lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

class CradleWindow;
class CradleGame;

class CradleD3D
{
public:
	CradleD3D();
	virtual ~CradleD3D();

	void Create(CradleWindow* window);

	void Resize(UINT width, UINT height);

	void SetFullscreen(BOOL enableFullscreen);

	void Clear(const float* color);

	ID3D11RenderTargetView* m_backBufferRenderTarget;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11Texture2D* m_depthStencilBuffer;

	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dContext;
	IDXGISwapChain* m_swapChain;

	D3D_DRIVER_TYPE m_driverType;
	D3D_FEATURE_LEVEL m_featureLevel;

	D3D11_VIEWPORT m_viewport;

	void CreateD3D(CradleWindow* window);

	HRESULT CompileShader(LPCWSTR sourceFile, LPCSTR entryPoint, LPCSTR profile, ID3DBlob** blob);




};

#endif