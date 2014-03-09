#include "CradleGame.h"
#include "CradleWindow.h"
#include "CrTexture2D.h" 
#include "CradleD3D.h"
#include "CradleFileManager.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <string>

#include "CradleVertexLayouts.h"

#include "ObjModel.h"

#include <list>
#include <unordered_map>
#include <strsafe.h>

#include <random>


#include <thread>



/*
#include <ShObjIdl.h>
#include <atlbase.h>
// #include <string.h>
// #include <io.h>
// #include <fcntl.h>
// #include <iostream>


HRESULT OpenFileDialog(CradleWindow* window)
{
	HRESULT hr;
	CComPtr<IFileOpenDialog> pDlg;
	COMDLG_FILTERSPEC aFileTypes[] =
	{
		{ L"Text files", L"*.txt" },
		{ L"Executable files", L"*.exe; *.dll" },
		{ L"All files", L"*.*" }
	};

	hr = pDlg.CoCreateInstance(__uuidof(FileOpenDialog));

	if ( FAILED(hr))
		return hr;

	pDlg->SetFileTypes(_countof(aFileTypes), aFileTypes);
	pDlg->SetTitle(L"A Single-Selection Dialog");

	hr = pDlg->Show(window->GetWindowHandle());

	if (SUCCEEDED(hr))
	{
		CComPtr<IShellItem> pItem;

		hr = pDlg->GetResult(&pItem);

		if (SUCCEEDED(hr))
		{
			LPWSTR str = nullptr;
			hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &str);	

			if (SUCCEEDED(hr))
			{
				// wchar_t* wide_str = L"Wide String";
				// printf("%s", wide_str);

				wprintf(L"Shell item name: %s\n", str);

				
				MessageBoxW(NULL, str, L"Caption", MB_OK);



				HANDLE hFile = CreateFile(str
							, GENERIC_READ
							, 0
							, NULL
							, OPEN_EXISTING
							, FILE_ATTRIBUTE_NORMAL
							, NULL);

				if (hFile == INVALID_HANDLE_VALUE)
				{
					std::cout << "Failed to open file\n";
				}
				else
				{
					
					DWORD dwBytesRead = 0;
					
					DWORD fileSize = GetFileSize(hFile, NULL);
					
					char* ReadBuffer = new char[fileSize];

					

					
					if (FALSE == ReadFile(hFile, ReadBuffer, fileSize-1, &dwBytesRead, NULL))
					{
						std::cout << "Failed to read file\n";
					

						CloseHandle(hFile);
					}
					else
					{
						if (dwBytesRead > 0 && dwBytesRead <= fileSize)
						{
							ReadBuffer[dwBytesRead] = '\0';

							std::wcout << ReadBuffer << std::endl;
						}
						else if (dwBytesRead == 0)
						{
							std::cout << "No data read from file\n";
						}
						else
						{
							std::cout << "Unexpected value for dwBytesRead\n";
						}

						CloseHandle(hFile);
					}


					delete [] ReadBuffer;

				}
			
				
				CoTaskMemFree(str);
			}

		}

	}

	return S_OK;
}

/* */

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);

void RegisterRedPanelClass(void);
void RegisterBluePanelClass(void);


HWND hWnd1 = 0;
HWND hWnd2 = 0;

LRESULT OnDestroy(CradleWindow& window, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
	return 0;
}

void PrintNumber(int* arr, int start, int count)
{
	for (int i = start; i < start + count; i++)
	{
		wprintf_s(L"i: %d\n", arr[i]);
	}
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	FILE* stream;	
	AllocConsole();
	freopen_s(&stream, "CONOUT$", "wb", stdout);
	int result = _setmode(_fileno(stdout), _O_U8TEXT);
	// int result = _setmode(_fileno(stdout), _O_U16TEXT);


	



	
	CradleWindow* w1 = new CradleWindow();

	w1->RegisterMessageHandler(WM_DESTROY, OnDestroy);
	w1->RegisterMessageHandler(WM_PAINT, CradleWindow::OnPaint);
	w1->RegisterMessageHandler(WM_SIZE, CradleWindow::OnSize);

	w1->Initialize(hInstance, cmdShow
					, L"WindowClass1", L"My Window"
					, WS_OVERLAPPEDWINDOW
					, 1280, 720, 300, 150
					, nullptr);
	





	

		

	CradleGame* game = new CradleGame();

	// CFTexture* texture = new CFTexture(w1->GetWindowHandle(), nullptr, nullptr, game->m_cd3d->m_d3dDevice);

	game->Init(w1);
	game->Start();

	delete game;
	delete w1;

	
	

	




	// CradleWindow* window1 = new CradleWindow();
	// window1->Initialize(hInstance, cmdShow, L"Window1Class", L"Window #1", WS_OVERLAPPEDWINDOW, nullptr);
	
	/*
	
	WNDCLASSEXW wndClass;
	ZeroMemory(&wndClass, sizeof(WNDCLASSEXW));

	wndClass.cbClsExtra = 0;
	wndClass.cbSize = sizeof(WNDCLASSEXW);
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)1;
	wndClass.hCursor = LoadCursor(0, IDC_ARROW);
	wndClass.hIcon = 0;
	wndClass.hIconSm = 0;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = L"WindowClass";
	wndClass.lpszMenuName = 0;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassExW(&wndClass);

	RECT rc = { 300, 200, 1024, 768 };
	AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);



	HWND hWnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, L"WindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN
		, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hWnd, cmdShow);
	UpdateWindow(hWnd);

	MSG msg = {0};

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	*/

	



	/*
	OpenFileDialog(window1);

	LPWSTR str = L"Foobar";

	std::wcout << str;

	// wprintf(L"%s", str);

	MessageBoxW(NULL, str, L"Caption", MB_OK);
	*/




	// while (window1->MessageLoop());

	// delete window1;


	
	
	
	/*
	WindowParameters wndParams = { hInstance, prevInstance, cmdLine, cmdShow };

	CradleGame* game = new CradleGame();
	game->Start(&wndParams);

	
	delete game;
	*/




	fclose( stream );
	FreeConsole();

	return 0;
}





HRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInst;
	RECT rect = {0};
	static HCURSOR hCursor;
	static BOOL bSplitterMoving;
	static DWORD dwSplitterPos;
	// static HWND hWnd1, hWnd2;

	switch (message)
	{

	case WM_CREATE:
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;

		RegisterRedPanelClass();
		RegisterBluePanelClass();

		hWnd1 = CreateWindowEx(0, L"RedPanelClass", nullptr
			, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS
			, 0, 0, 0, 0
			, hWnd, (HMENU)1
			, hInst, nullptr);

		hWnd2 = CreateWindowEx(0, L"BluePanelClass", nullptr
			, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS
			, 0, 0, 0, 0
			, hWnd, (HMENU)2
			, hInst, nullptr);

		hCursor = LoadCursor(nullptr, MAKEINTRESOURCE(IDC_SIZENS));
		bSplitterMoving = FALSE;

		dwSplitterPos = 130;
		return 0;

	case WM_SIZE:

		if ((wParam != SIZE_MINIMIZED) && (HIWORD(lParam) < dwSplitterPos))
			dwSplitterPos = HIWORD(lParam) - 10;

		MoveWindow(hWnd1, 0, 0, LOWORD(lParam), dwSplitterPos - 1, TRUE);
		MoveWindow(hWnd2, 0, dwSplitterPos + 2, LOWORD(lParam), HIWORD(lParam) - dwSplitterPos - 2, TRUE);

		GetClientRect(hWnd1, &rect);
		

		wprintf_s(L"First client area: (%d, %d, %d, %d)\n", rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);

		GetClientRect(hWnd2, &rect);
		

		wprintf_s(L"Second client area: (%d, %d, %d, %d)\n\n", rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);



		return 0;

	case WM_MOUSEMOVE:
		if (HIWORD(lParam) > 10 && HIWORD(lParam) < 400)
		{
			SetCursor(hCursor);
			if ((wParam == MK_LBUTTON) && bSplitterMoving)
			{
				GetClientRect(hWnd, &rect);
				if (HIWORD(lParam) > rect.bottom)
					return 0;

				dwSplitterPos = HIWORD(lParam);
				SendMessage(hWnd, WM_SIZE, 0, MAKELPARAM(rect.right, rect.bottom));


			}
		}

	case WM_MOUSELEAVE:

		SetCursor(hCursor);
		return 0;

	case WM_LBUTTONDOWN:
		SetCursor(hCursor);
		bSplitterMoving = TRUE;
		SetCapture(hWnd);
		return 0;

	case WM_LBUTTONUP:
		ReleaseCapture();
		bSplitterMoving = FALSE;

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcW(hWnd, message, wParam, lParam);
	
}

HRESULT CALLBACK ChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)  
  {
    case WM_LBUTTONUP:
    
      // Beep(50, 40);
      break;    
  }

  return DefWindowProcW(hWnd, message, wParam, lParam);
}

void RegisterRedPanelClass(void) 
{
  HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));

  WNDCLASSW rwc = {0};
  rwc.lpszClassName = L"RedPanelClass";
  rwc.hbrBackground = hbrush;
  rwc.lpfnWndProc   = ChildProc;
  rwc.hCursor       = LoadCursor(0, IDC_ARROW);
  RegisterClassW(&rwc); 
}

void RegisterBluePanelClass(void) 
{
  HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 255));

  WNDCLASSW rwc = {0};
  rwc.lpszClassName = L"BluePanelClass";
  rwc.hbrBackground = hbrush;
  rwc.lpfnWndProc   = ChildProc;
  rwc.hCursor       = LoadCursor(0, IDC_ARROW);

  RegisterClassW(&rwc);
}