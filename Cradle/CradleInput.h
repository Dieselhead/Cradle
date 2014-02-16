#ifndef _CRADLEINPUT_
#define _CRADLEINPUT_

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include <dinput.h>

#define MOUSE_ONE 0
#define MOUSE_TWO 1
#define MOUSE_THREE 2
#define MOUSE_FOUR 3
#define MOUSE_FIVE 4

class CradleGame;

class CradleInput
{
public:
	CradleInput(CradleGame* game);
	virtual ~CradleInput();

	IDirectInputDevice8* m_diKeyboard;
	IDirectInputDevice8* m_diMouse;

	LPDIRECTINPUT8 m_directInput;

	void Initialize();
	void Update();

	BOOL KeyDown(const char key);
	BOOL KeyUp(const char key);
	BOOL KeyPressed(const char key);
	BOOL KeyReleased(const char key);

	BOOL MouseDown(const char key);
	BOOL MouseUp(const char key);
	BOOL MousePressed(const char key);
	BOOL MouseReleased(const char key);

	LONG MouseWheelDelta();
	LONG MouseXDelta();
	LONG X();
	LONG MouseYDelta();
	LONG Y();

	

	CradleGame* m_game;

	DIMOUSESTATE2 m_currentMouseState;
	DIMOUSESTATE2 m_lastMouseState;

	BYTE* m_currentKeyboardState;
	BYTE* m_lastKeyboardState;


	bool wGetKeyPress(unsigned char key);

	unsigned char* m_wCurrentKeyboardState;
	unsigned char* m_wLastKeyboardState;
};

#endif