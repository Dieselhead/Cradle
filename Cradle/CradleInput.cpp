#include "CradleInput.h"
#include "CradleGame.h"
#include "CradleWindow.h"

#include <stdio.h>

CradleInput::CradleInput(CradleGame* game)
{
	m_game = game;
	m_currentKeyboardState = 0;
	m_lastKeyboardState = 0;

	m_wCurrentKeyboardState = 0;
	m_wLastKeyboardState = 0;;
}

CradleInput::~CradleInput()
{
	if (m_directInput)
	{
		if (m_diKeyboard)
		{
			m_diKeyboard->Unacquire();
			m_diKeyboard->Release();
			m_diKeyboard = nullptr;
		}

		if (m_diMouse)
		{
			m_diMouse->Unacquire();
			m_diMouse->Release();
			m_diMouse = nullptr;
		}

		m_directInput->Release();
		m_directInput = nullptr;
	}

	if (m_currentKeyboardState) delete [] m_currentKeyboardState;
	if (m_lastKeyboardState) delete [] m_lastKeyboardState;

	if (m_wCurrentKeyboardState) delete [] m_wCurrentKeyboardState;
	if (m_wLastKeyboardState) delete [] m_wLastKeyboardState;
}

BOOL CradleInput::KeyDown(const char key)
{
	return (m_currentKeyboardState[key] & 0x80);
}

BOOL CradleInput::KeyUp(const char key)
{
	return !(m_currentKeyboardState[key] & 0x80);
}

BOOL CradleInput::KeyPressed(const char key)
{
	return ((m_currentKeyboardState[key] & 0x80) && !(m_lastKeyboardState[key] & 0x80));
}

BOOL CradleInput::KeyReleased(const char key)
{
	return (!(m_currentKeyboardState[key] & 0x80) && (m_lastKeyboardState[key] & 0x80));
}


BOOL CradleInput::MouseDown(const char key)
{
	return (m_currentMouseState.rgbButtons[key] & 0x80);
}

BOOL CradleInput::MouseUp(const char key)
{
	return !(m_currentMouseState.rgbButtons[key] & 0x80);
}

BOOL CradleInput::MousePressed(const char key)
{
	return ((m_currentMouseState.rgbButtons[key] & 0x80) && !(m_lastMouseState.rgbButtons[key] & 0x80));
}

BOOL CradleInput::MouseReleased(const char key)
{
	return (!(m_currentMouseState.rgbButtons[key] & 0x80) && (m_lastMouseState.rgbButtons[key] & 0x80));
}

LONG CradleInput::MouseWheelDelta()
{
	return m_currentMouseState.lZ;
}

LONG CradleInput::MouseXDelta()
{
	return m_currentMouseState.lX;
}

LONG CradleInput::X()
{
	return MouseXDelta();
}

LONG CradleInput::MouseYDelta()
{
	return m_currentMouseState.lY;
}

LONG CradleInput::Y()
{
	return MouseYDelta();
}


void CradleInput::Update()
{
	m_diKeyboard->Acquire();
	m_diMouse->Acquire();

	BYTE* temp;
	temp = m_lastKeyboardState;
	m_lastKeyboardState = m_currentKeyboardState;
	m_currentKeyboardState = temp;
	ZeroMemory(m_currentKeyboardState, 256);

	m_lastMouseState = m_currentMouseState;

	m_diKeyboard->GetDeviceState(256, (LPVOID)m_currentKeyboardState);
	m_diMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&m_currentMouseState);






	unsigned char* wTemp;
	wTemp = m_wLastKeyboardState;
	m_wLastKeyboardState = m_wCurrentKeyboardState;
	m_wCurrentKeyboardState = wTemp;
	ZeroMemory(m_wCurrentKeyboardState, 256);

	GetKeyboardState(m_wCurrentKeyboardState);
}

bool CradleInput::wGetKeyPress(unsigned char key)
{
	return ((m_wCurrentKeyboardState[key] & 0x80) && !(m_wLastKeyboardState[key] & 0x80));
}

void CradleInput::Initialize()
{

	HRESULT hr;

	hr = DirectInput8Create(m_game->m_cWindow->GetInstance()
			, DIRECTINPUT_VERSION
			, IID_IDirectInput8
			, (void**)&m_directInput, 0);

	if (FAILED(hr))
	{
		// TODO
		// Exit
		printf("Failed to created DirectInput object");
	}

	m_directInput->CreateDevice(GUID_SysKeyboard, &m_diKeyboard, 0);
	m_directInput->CreateDevice(GUID_SysMouse, &m_diMouse, 0);

	m_diKeyboard->SetDataFormat(&c_dfDIKeyboard);
	m_diKeyboard->SetCooperativeLevel(
		m_game->m_cWindow->GetWindowHandle()
		, DISCL_FOREGROUND | DISCL_NOWINKEY | DISCL_NONEXCLUSIVE);


	m_diMouse->SetDataFormat(&c_dfDIMouse2);
	m_diMouse->SetCooperativeLevel(
		m_game->m_cWindow->GetWindowHandle()
		, DISCL_FOREGROUND | DISCL_NOWINKEY | DISCL_NONEXCLUSIVE);

	hr = m_diKeyboard->Acquire();

	if (FAILED(hr))
	{
		// TODO
		// Handle
		printf("Failed to aquire keyboard");
	}

	hr = m_diMouse->Acquire();

	if (FAILED(hr))
	{
		// TODO
		// Handle
		printf("Failed to aquire mouse");
	}

	m_currentKeyboardState = new BYTE[256];
	m_lastKeyboardState = new BYTE[256];


	m_wCurrentKeyboardState = new unsigned char[256];
	m_wLastKeyboardState = new unsigned char[256];
}