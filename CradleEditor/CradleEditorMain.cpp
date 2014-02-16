#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include <CradleWindow.h>
#include <CradleGame.h>

#include <ShObjIdl.h>

#include <stdlib.h>


#define IDC_BTN_LOADMODEL 5001
#define IDBC_TWO 5002
#define IDC_LB_CHILDREN 5003
#define IDC_ST_CHILD_DATA 5004

#define IDC_BTN_LOADTEXTURE 6001
#define IDC_LV_LOADEDTEXTURES 6002
#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")



LRESULT OnCreate(CradleWindow& window, HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OnCommand(CradleWindow& window, HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OnSize(CradleWindow& window, HWND hWnd, WPARAM wParam, LPARAM lParam);

LRESULT OnTextureNCCreate(CradleWindow& window, HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OnTextureCreate(CradleWindow& window, HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OnTextureCommand(CradleWindow& window, HWND hWnd, WPARAM wParam, LPARAM lParam);

CradleWindow* mainWindow;

CradleWindow* textureLoadingWindow;

void LoadModel(CradleWindow& window);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	FILE* stream;

	AllocConsole();
	// freopen( "CONOUT$", "wb", stdout);
	freopen_s(&stream, "CONOUT$", "wb", stdout);
	int result = _setmode(_fileno(stdout), _O_U8TEXT);


	/*
	textureLoadingWindow = new CradleWindow();


	textureLoadingWindow->RegisterMessageHandler(WM_DESTROY, CradleWindow::OnDestroy);
	textureLoadingWindow->RegisterMessageHandler(WM_SIZE, CradleWindow::OnSize);
	textureLoadingWindow->RegisterMessageHandler(WM_PAINT, CradleWindow::OnPaint);
	textureLoadingWindow->RegisterMessageHandler(WM_NCCREATE, OnTextureNCCreate);
	textureLoadingWindow->RegisterMessageHandler(WM_CREATE, OnTextureCreate);
	textureLoadingWindow->RegisterMessageHandler(WM_COMMAND, OnTextureCommand);
	textureLoadingWindow->Initialize(hInstance, cmdShow, L"TextureLoadingWindowClass", L"Texture Loading", WS_OVERLAPPEDWINDOW, 1280, 720, 100, 100, nullptr);

	while (textureLoadingWindow->MessageLoop());

	delete textureLoadingWindow;

	*/

	
	mainWindow = new CradleWindow();
	mainWindow->RegisterMessageHandler(WM_DESTROY, CradleWindow::OnDestroy);
	mainWindow->RegisterMessageHandler(WM_SIZE, CradleWindow::OnSize);
	mainWindow->RegisterMessageHandler(WM_PAINT, CradleWindow::OnPaint);
	mainWindow->RegisterMessageHandler(WM_CREATE, OnCreate);
	mainWindow->RegisterMessageHandler(WM_COMMAND, OnCommand);

	mainWindow->Initialize(hInstance, cmdShow, L"EditorWindow", L"Editor"
				, WS_OVERLAPPEDWINDOW | WS_VISIBLE
				, 1280, 720, 100, 100, nullptr);

	CradleWindow* outputWindow = new CradleWindow();
	outputWindow->RegisterMessageHandler(WM_DESTROY, CradleWindow::OnDestroy);
	outputWindow->RegisterMessageHandler(WM_SIZE, CradleWindow::OnSize);
	outputWindow->RegisterMessageHandler(WM_PAINT, CradleWindow::OnPaint);
	outputWindow->Initialize(hInstance, cmdShow, L"OutputWindow"
		, L"Output", WS_CHILD | WS_VISIBLE, 640, 480, 5, 5, mainWindow);

	CradleGame* game = new CradleGame();
	game->Init(outputWindow);
	game->Start();
	


	// while (wnd->MessageLoop());

	delete game;
	delete mainWindow;
	delete outputWindow;
	

	fclose(stream);
	FreeConsole();
	
	return 0;
}

void LoadFBXModel(char* filename)
{
	/*
	FbxManager* manager = FbxManager::Create();
	if (manager)
	{
		FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);


		size_t i;




		manager->SetIOSettings(ios);

		FbxImporter* importer = FbxImporter::Create(manager, "");


		importer->Initialize(filename, -1, manager->GetIOSettings());


		FbxScene* scene = FbxScene::Create(manager, "test scene");
		importer->Import(scene);

		FbxNode* rootNode = scene->GetRootNode();


		LRESULT lr = SendMessage(mainWindow->GetControl("ChildList"), LB_GETCOUNT, 0, 0);

		for (int i = 0; i < lr; i++)
		{
			SendMessage(mainWindow->GetControl("ChildList"), LB_DELETESTRING, 0, 0);
		}

		if (rootNode)
		{
			wprintf(L"There is a root node\n");
			wprintf(L"Child count: %d\n", rootNode->GetChildCount());

			wchar_t* wstring = new wchar_t[256];
			for (int i = 0; i < rootNode->GetChildCount(); i++)
			{
				FbxNode* child = rootNode->GetChild(i);
				mbstowcs(wstring, child->GetName(), 256);

				SendMessage(mainWindow->GetControl("ChildList"), LB_ADDSTRING
					, 0, (LPARAM)wstring);

				for (int j = 0; j < child->GetNodeAttributeCount(); j++)
				{
					FbxNodeAttribute* attribute = child->GetNodeAttributeByIndex(j);

					wprintf(L"Node attribute: %d\n", attribute->GetAttributeType());
				}


			}


			wprintf(L"Number of stuffs %d\n", lr);



			delete[] wstring;

		}

		if (importer)
			importer->Destroy();
	}

	if (manager)
		manager->Destroy();
	
	*/

}

void LoadModel(CradleWindow& window)
{
	/*
	CComPtr<IFileOpenDialog> pFileDialog;
	HRESULT hr = pFileDialog.CoCreateInstance(__uuidof(FileOpenDialog));
	// HRPRINT(hr, L"CoCreate failed\n", L"CoCreate succeeded");

	if (SUCCEEDED(hr))
	{
		hr = pFileDialog->Show(window.GetWindowHandle());
		
	
		if (SUCCEEDED(hr))
		{
			IShellItem* pItem;
			hr = pFileDialog->GetResult(&pItem);

			if (SUCCEEDED(hr))
			{
				LPWSTR str;

				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &str);
				

				if (SUCCEEDED(hr))
				{
					wprintf(L"%s\n", str);

					FbxManager* manager = FbxManager::Create();
					if (manager)
					{
						FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
						
						char* cstr = new char[256];

						size_t i;

						wcstombs_s(&i, cstr, (size_t)256,  str, size_t(256));


						
						manager->SetIOSettings(ios);

						FbxImporter* importer = FbxImporter::Create(manager, "");


						importer->Initialize(cstr, -1, manager->GetIOSettings());

						delete cstr;

						FbxScene* scene = FbxScene::Create(manager, "test scene");
						importer->Import(scene);

						FbxNode* rootNode = scene->GetRootNode();
						

						LRESULT lr = SendMessage(mainWindow->GetControl("ChildList"), LB_GETCOUNT, 0, 0);

						for (int i = 0; i < lr; i++)
						{
							SendMessage(mainWindow->GetControl("ChildList"), LB_DELETESTRING, 0, 0);
						}

						if (rootNode)
						{
							wprintf(L"There is a root node\n");
							wprintf(L"Child count: %d\n", rootNode->GetChildCount());
							
							wchar_t* wstring = new wchar_t[256];
							for (int i = 0; i < rootNode->GetChildCount(); i++)
							{
								FbxNode* child = rootNode->GetChild(i);
								mbstowcs(wstring, child->GetName(), 256);

								SendMessage(mainWindow->GetControl("ChildList"), LB_ADDSTRING
									, 0, (LPARAM)wstring);

								for (int j = 0; j < child->GetNodeAttributeCount(); j++)
								{
									FbxNodeAttribute* attribute = child->GetNodeAttributeByIndex(j);
									
									wprintf(L"Node attribute: %d\n", attribute->GetAttributeType());
								}
								

							}

							
							wprintf(L"Number of stuffs %d\n", lr);



							delete [] wstring;

						}
						
						if (importer)
							importer->Destroy();
					}



					if (manager)
						manager->Destroy();
				}

				CoTaskMemFree(str);
				pItem->Release();
			}
		}
	}
	*/
}



LRESULT OnCreate(CradleWindow& window, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	wprintf(L"On Create.\n");

	if (hWnd)
	{
		wprintf(L"hWnd is ok\n");
	}
	else
	{
		wprintf(L"hWnd is not ok\n");
	}

	HWND btn = CreateWindowEx(0, L"BUTTON", L"Load Model", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
		, 5, 490, 100, 25, hWnd, (HMENU)IDC_BTN_LOADMODEL, NULL, NULL);

	HWND childList = CreateWindowEx(0, L"LISTBOX", L"Children"
		, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_HASSTRINGS
		, 650, 5, 200, 400, hWnd, (HMENU)IDC_LB_CHILDREN, NULL, NULL);

	HWND childData = CreateWindowEx(0, L"STATIC", L"Select a Child node to view data."
		, WS_CHILD | WS_VISIBLE | WS_BORDER, 650 + 5 + 200, 5, 200, 400
		, hWnd, (HMENU)IDC_ST_CHILD_DATA, NULL, NULL);

	SendMessage(childList, LB_ADDSTRING, 0, (LPARAM)L"Test");
	

	window.AddControl("ChildList", childList);
	window.AddControl("StaticChildData", childData);

	window.AddControl("Button One", btn);
	
	UpdateWindow(window.GetWindowHandle());

	// ShowWindow(window.GetWindowHandle(), 1);

	return DefWindowProc(window.GetWindowHandle(), WM_CREATE, wParam, lParam);
}

LRESULT OnSize(CradleWindow& window, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	bool b = true;

	return 0;
}

LRESULT OnCommand(CradleWindow& window, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(wParam) == IDC_BTN_LOADMODEL)
	{
		// LoadFBXModel("blender_fbxtest.fbx");
	}
	else if (LOWORD(wParam) == IDC_LB_CHILDREN)
	{
		if (HIWORD(wParam) == LBN_SELCHANGE)
		{
			


		}

	}

	return 0;
}

LRESULT OnTextureNCCreate(CradleWindow& window, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	wprintf(L"OnTextureNCCreate\n");

	return DefWindowProc(hWnd, WM_NCCREATE, wParam, lParam);
}

LRESULT OnTextureCreate(CradleWindow& window, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	wprintf(L"OnTextureCreate\n");

	// Create button to select file to load

	HWND btnSelectFile = CreateWindowEx(0, L"BUTTON", L"btnSelectFile"
						, WS_CHILD | WS_VISIBLE
						, 5, 5, 90, 25, hWnd, (HMENU)IDC_BTN_LOADTEXTURE, nullptr, nullptr);

	window.AddControl("btnSelectFile", btnSelectFile);

	HWND lvTextures = CreateWindowEx(0, WC_LISTVIEW, L"lvTextures"
						, WS_CHILD | WS_VISIBLE | LVS_SINGLESEL | LVS_REPORT
						, 5, 35, 480, 320, hWnd, (HMENU)IDC_LV_LOADEDTEXTURES, nullptr, nullptr);

	window.AddControl("lvTextures", lvTextures);

	LVCOLUMN col = {0};
	col.mask = LVCF_TEXT;
	col.cx = 240;
	col.iOrder = 0;
	col.cxMin = 140;
	col.pszText = L"Name";

	ListView_InsertColumn(lvTextures, col.iOrder, &col);

	LVITEM item = {0};
	item.mask = LVIF_TEXT;
	item.pszText = L"Foobar";
	item.cchTextMax = lstrlenW(L"Foobar");
	item.iItem = SendMessage(lvTextures, LVM_GETITEMCOUNT, 0, 0);

	ListView_InsertItem(lvTextures, &item);
	ListView_EnsureVisible(lvTextures, item.iItem, 0);

	UpdateWindow(window.GetWindowHandle());

	// Create list to show already loaded files

	return DefWindowProc(window.GetWindowHandle(), WM_CREATE, wParam, lParam);
}

LRESULT OnTextureCommand(CradleWindow& window, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	wprintf(L"OnTextureCommand\n");

	switch (LOWORD(wParam))
	{
	case IDC_BTN_LOADTEXTURE:

		break;
	case IDC_LV_LOADEDTEXTURES:

		break;

	}
	return DefWindowProc(hWnd, WM_COMMAND, wParam, lParam);
}