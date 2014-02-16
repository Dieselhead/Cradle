#ifndef _CRADLEFILE_
#define _CRADLEFILE_
#include <ShObjIdl.h>

#define HRPRINT(hr, failed, success) wprintf((FAILED(hr)) ? failed : success);

class CradleFile
{
public:
	CradleFile(HWND, COMDLG_FILTERSPEC*, LPWSTR);
	virtual ~CradleFile();

	void PickFile(/* Valid file extensions */);
	virtual HRESULT LoadFile(/* FileName */) = 0;
	
	void SaveFile(const char* fileName, const char* data);
	void PutFile();

	
	
protected:
	HRESULT OpenPickFileDialog(/* File extensions */);

	HWND m_hWnd;
	LPWSTR m_filePath;
	COMDLG_FILTERSPEC* m_fileFilter;


};
#endif