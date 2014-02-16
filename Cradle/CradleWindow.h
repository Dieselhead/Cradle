#ifndef _CRADLEWINDOW_
#define _CRADLEWINDOW_
#include <Windows.h>
#include <unordered_map>

class CradleWindow;

__declspec(dllexport) typedef LRESULT (*tyMessageHandler) (CradleWindow&, HWND, WPARAM, LPARAM);
__declspec(dllexport) typedef std::unordered_map<UINT, tyMessageHandler> tyMessageMap;
__declspec(dllexport) typedef tyMessageMap::iterator tyMessageIterator;


class __declspec(dllexport) CradleWindow
{
public:
	
	CradleWindow();
	virtual ~CradleWindow();

	bool Initialize(HINSTANCE hInstance, int cmdShow, LPWSTR className, LPWSTR windowName, LONG_PTR windowStyle, int windowWidth, int windowHeight, int x, int y, CradleWindow* parent);
	bool ResizeAndMove(int width, int height, int x, int y);
	bool Move(int x, int y);
	bool Resize(int width, int height);

	bool SetWindowStyle(LONG_PTR newStyle);

	bool MessageLoop();

	int GetWidth();
	int GetHeight();
	int GetClientWidth();
	int GetClientHeight();

	int GetX();
	int GetY();
	HWND GetWindowHandle();
	HINSTANCE GetInstance();

	UINT HandleMessage();


	LONG_PTR m_currentWindowStyle;

	void RegisterMessageHandler(UINT message, tyMessageHandler handler);
	BOOL GetMessageHandler(UINT message, tyMessageHandler& handler);

	static LRESULT OnPaint(CradleWindow&, HWND, WPARAM, LPARAM);
	static LRESULT OnSize(CradleWindow&, HWND, WPARAM, LPARAM);
	static LRESULT OnDestroy(CradleWindow&, HWND, WPARAM, LPARAM);

	void AddControl(const char* key, HWND hWnd);
	HWND GetControl(const char* key);

	

private:

	HWND m_hwnd;
	HINSTANCE m_hInstance;

	int m_width;
	int m_height;
	int m_clientWidth;
	int m_clientHeight;

	int m_windowedX;
	int m_windowedY;

	LONG_PTR m_windowStyle;

	virtual void SetDefaultWindow();

	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	tyMessageMap m_messageMap;
	std::unordered_map<const char*, HWND> m_controls;
	

protected:
	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	MSG m_msg;
	
};



#endif