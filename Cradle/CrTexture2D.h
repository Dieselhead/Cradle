#ifndef _CFTEXTURE_
#define _CFTEXTURE_
#include "CradleFile.h"
#include <d3d11.h>

class CrTexture2D : CradleFile
{
public:
	CrTexture2D(HWND, COMDLG_FILTERSPEC*, LPWSTR, ID3D11Device*);
	virtual ~CrTexture2D();

	
	ID3D11ShaderResourceView* m_colorData;

	inline ID3D11ShaderResourceView* GetColorData() { return m_colorData; }
	virtual HRESULT LoadFile();
	

protected:
	HRESULT LoadFileDDS();

	ID3D11Device* m_d3dDevice;
};

#endif