#include "CradleWindow.h"
#include "CradleGame.h"

#include <stdio.h>

CradleWindow::CradleWindow()
{
	SetDefaultWindow();

	ZeroMemory(&m_msg, sizeof(MSG));

}

CradleWindow::~CradleWindow()
{
	
}

void CradleWindow::SetDefaultWindow()
{
	// Load settings
	// Check capabilities
	// etc

	m_width = 640;
	m_height = 480;
	m_windowedX = 100;
	m_windowedY = 100;
	m_windowStyle = WS_OVERLAPPEDWINDOW;

}

bool CradleWindow::Resize(int width, int height)
{
	/*
	SetWindowPos(m_hwnd, HWND_TOP, m_windowedX, m_windowedY, width, height, SWP_NOMOVE);

	m_width = width;
	m_height = height;
	*/

	return ResizeAndMove(width, height, m_windowedX, m_windowedY);
}

bool CradleWindow::Move(int x, int y)
{
	/*
	SetWindowPos(m_hwnd, HWND_TOP, x, y, m_width, m_height, SWP_NOSIZE);

	m_windowedX = x;
	m_windowedY = y;
	*/


	return ResizeAndMove(m_width, m_height, x, y);
}

bool CradleWindow::ResizeAndMove(int width, int height, int x, int y)
{
	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, m_currentWindowStyle, false);

	m_windowedX = x;
	m_windowedY = y;
	m_width = rc.right - rc.left;
	m_height = rc.bottom - rc.top;

	SetWindowPos(m_hwnd, HWND_TOP, m_windowedX, m_windowedY, m_width, m_height, 0);
	
	return true;
}

bool CradleWindow::SetWindowStyle(LONG_PTR newStyle)
{
	SetWindowLongPtr(m_hwnd, GWL_STYLE, newStyle);
	ShowWindow(m_hwnd, 1);

	m_windowStyle = newStyle;

	return true;
}

/*
bool CradleWindow::ChangeWindowType(WindowType newType)
{
	// WS_POPUP for borderless window and fullscreenwindowed

	// WS_OVERLAPPEDWINDOW for fullscreen and windowed



	if (newType == WindowTypeWindowed)
	{
		m_currentWindowStyle = m_styleWindowed;
	}
	else if (newType == WindowTypeWindowedBorderless)
	{
		m_currentWindowStyle = m_styleWindowedBorderless;
	}
	else if (newType == WindowTypeFullscreen)
	{
		m_currentWindowStyle = m_styleFullscreen;
	}
	else if (newType == WindowTypeWindowedFullscreen)
	{
		m_currentWindowStyle = m_styleWindowedFullscreen;
	}

	m_windowType = newType;

	SetWindowLongPtr(m_hwnd, GWL_STYLE, m_currentWindowStyle);
	ShowWindow(m_hwnd, 1);



	return true;
}
*/

bool CradleWindow::Initialize(HINSTANCE hInstance, int cmdShow, LPWSTR className, LPWSTR windowName, LONG_PTR windowStyle, int windowWidth, int windowHeight, int x, int y, CradleWindow* parent)
{
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = CradleWindow::StaticWndProc;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpszClassName = className;
	wndClass.lpszMenuName = NULL;
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

	if (!RegisterClassEx(&wndClass))
		return false;


	RECT rc = { 0, 0, windowWidth, windowHeight };
	AdjustWindowRect(&rc, windowStyle, false);

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	HWND hwnd = CreateWindow(className, windowName, windowStyle
		, x, y, width, height
		, (parent) ? parent->GetWindowHandle() : NULL, NULL, hInstance, (void*)this);

	
	GetWindowRect(hwnd, &rc);

	m_windowedX = x;
	m_windowedY = y;

	if (!hwnd)
		return false;

	ShowWindow(hwnd, cmdShow);
	UpdateWindow(hwnd);

	m_hwnd = hwnd;
	m_hInstance = hInstance;
	m_windowStyle = windowStyle;
	m_windowedX = x;
	m_windowedY = y;
	m_clientWidth = windowWidth;
	m_clientHeight = windowHeight;
	m_width = rc.right - rc.left;
	m_height = rc.bottom - rc.top;

	return true;
}

LRESULT CradleWindow::OnDestroy(CradleWindow& window, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);

	return 0;
}

LRESULT CradleWindow::OnPaint(CradleWindow& window, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hDc;

	hDc = BeginPaint(hWnd, &paintStruct);
	EndPaint(hWnd, &paintStruct);

	return 0;
}

LRESULT CradleWindow::OnSize(CradleWindow& window, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case SIZE_RESTORED:
		wprintf(L"Size restored.\n");
		break;
	case SIZE_MINIMIZED:
		wprintf(L"Size minimized.\n");
		break;
	case SIZE_MAXSHOW:
		wprintf(L"Size max show.\n");
		break;
	case SIZE_MAXIMIZED:
		wprintf(L"Size maximized.\n");
		break;
	case SIZE_MAXHIDE:
		wprintf(L"Size max hide.\n");
		break;

	}

	WORD width = LOWORD(lParam);
	WORD height = HIWORD(lParam);

	wprintf(L"width: %d, height: %d\n", width, height);

	return 0;
}

void CradleWindow::RegisterMessageHandler(UINT message, tyMessageHandler handler)
{
	m_messageMap[message] = handler;
}

BOOL CradleWindow::GetMessageHandler(UINT message, tyMessageHandler& handler)
{
	tyMessageIterator it = m_messageMap.find(message);

	if (it == m_messageMap.end())
		return FALSE;
	
	
	handler = it->second;
	return TRUE;
}

LRESULT CALLBACK CradleWindow::StaticWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CradleWindow* thisWindow = nullptr;

	if (message == WM_NCCREATE)
	{
		SetWindowLongPtr(hwnd, GWL_USERDATA, (long)((LPCREATESTRUCT)lParam)->lpCreateParams);
		wprintf(L"WM_NCCREATE\n");
	}

	thisWindow = (CradleWindow*)GetWindowLongPtr(hwnd, GWL_USERDATA);

	if (thisWindow)
	{
		// return thisWindow->WndProc(hwnd, message, wParam, lParam);

		tyMessageHandler handler;
		if (thisWindow->GetMessageHandler(message, handler))
		{
			return (handler)((*thisWindow), hwnd, wParam, lParam);
		}
		else
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	else
		return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK CradleWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hDc;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		hDc = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
		break;
	case WM_SIZE:
		DefWindowProc(hwnd, message, wParam, lParam);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);

	}

	return 0;
}

UINT CradleWindow::HandleMessage()
{
	if (PeekMessage(&m_msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
	
		return m_msg.message;
	}

	return 0;
}

bool CradleWindow::MessageLoop()
{
	MSG msg = { 0 };

	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
		{
			return false;
		}
	}
	else
	{
		
	}

	return true;
}

void CradleWindow::AddControl(const char* key, HWND hWnd)
{
	m_controls[key] = hWnd;
}

HWND CradleWindow::GetControl(const char* key)
{
	if (m_controls.find(key) != m_controls.end())
		return m_controls[key];

	return nullptr;
}

int CradleWindow::GetWidth()
{
	return m_width;
}

int CradleWindow::GetHeight()
{
	return m_height;
}

HWND CradleWindow::GetWindowHandle()
{
	return m_hwnd;
}

HINSTANCE CradleWindow::GetInstance()
{
	return m_hInstance;
}

int CradleWindow::GetX()
{
	return m_windowedX;
}

int CradleWindow::GetY()
{
	return m_windowedY;
}

int CradleWindow::GetClientWidth()
{
	return m_clientWidth;
}

int CradleWindow::GetClientHeight()
{
	return m_clientHeight;
}