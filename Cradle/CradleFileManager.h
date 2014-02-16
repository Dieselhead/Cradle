#ifndef _CRADLEFILEMANAGER_
#define _CRADLEFILEMANAGER_

#include <Windows.h>
#include <stdio.h>
#include <unordered_map>
#include "CradleD3D.h"
#include <DirectXMath.h>
#include <list>

class CradleResource;

struct Foo
{
	ID3D11VertexShader* VS;
	ID3D11InputLayout* IL;
};

class CradleFileManager
{
public:
	__declspec(dllexport) CradleFileManager(CradleD3D*);
	virtual ~CradleFileManager();

	__declspec(dllexport) bool GetFilenames();

	__declspec(dllexport) bool LoadShaders();
	__declspec(dllexport) ID3D11PixelShader* GetPixelShader(wchar_t*);
	__declspec(dllexport) ID3D11VertexShader* GetVertexShader(wchar_t*);
	__declspec(dllexport) ID3D11InputLayout* GetInputLayout(wchar_t*);


	__declspec(dllexport) bool LoadMeshes();
	bool AddMesh(CradleResource* mesh);

	__declspec(dllexport) CradleResource* GetMesh(std::wstring);


	__declspec(dllexport) CradleD3D* GetCD3D() { return m_cd3d; }


	void DrawMesh(std::wstring name, DirectX::XMFLOAT4X4* matrix);
	void Draw();


	const wchar_t* m_resourcePath = L"D:\\Dropbox\\Dropbox\\_DirectX\\Source\\Cradle\\Debug\\Content\\Shaders\\";


protected:
	std::unordered_map<wchar_t*, ID3D11PixelShader*> m_pixelShaders;
	std::unordered_map<wchar_t*, Foo*> m_vertexShaders;

	std::unordered_map<std::wstring, CradleResource*> m_meshes;

	std::unordered_map<std::wstring, std::list<DirectX::XMFLOAT4X4*>> m_meshesToDraw;

	CradleD3D* m_cd3d;
	
};

#endif