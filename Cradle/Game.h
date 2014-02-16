#ifndef _GAME_
#define _GAME_

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Timer.h"
#include "FbxModel.h"
#include "FbxModelLoader.h"
#include "Camera.h"


enum WindowType
{
	Windowed
	, FullScreen
	, WindowedFullScreen
	, WindowedBorderless
};

struct VertexPos
{
	DirectX::XMFLOAT4 Position;
};

struct cbPerObject
{
	DirectX::XMMATRIX WPV;
};

class Game
{
	
/*
used when creating window and backbuffer / swapchain
	width
	height

used when creating swapchain
	double / tripple buffer
	refresh rate (hz)
	windowed/fullscreen
	multisample


*/

public:
	Game();
	virtual ~Game();

	bool Initialize(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow);
	virtual void LoadContent();
	virtual void UnloadContent();

	void Update();
	virtual void Render();
	virtual void Update(double total, double delta);

	void Shutdown();
	void Start();


	WindowType GetWindowType() { return m_windowType; }

	
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	
	void MessageLoop();

	Timer* m_timer;

	bool InitializeWindow(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow);
	bool InitializeDirectX();

	void Resize(unsigned int width, unsigned int height);


	bool temp;


	HWND m_hWnd;
	HINSTANCE m_hInstance;

	WindowType m_windowType;
	unsigned int m_windowWidth;
	unsigned int m_windowHeight;
	int m_windowX;
	int m_windowY;


	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dContext;
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_backbufferTarget;
	D3D_DRIVER_TYPE m_driverType;
	D3D_FEATURE_LEVEL m_featureLevel;

	FbxModelLoader* m_modelLoader;
	Model* m_testModel;

	ID3D11PixelShader* m_pixelShader;
	ID3D11VertexShader* m_vertexShader;
	ID3D11InputLayout* m_inputLayout;
	ID3D11Buffer* m_vertexBuffer;

	ID3D11Buffer* m_cbPerObjectBuffer;
	cbPerObject cbPerObj;

	Camera* m_camera;

	ID3D11RasterizerState* m_rsWireFrame;
};

#endif