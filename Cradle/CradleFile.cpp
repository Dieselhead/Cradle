#include "CradleFile.h"

CradleFile::CradleFile(HWND hWnd, COMDLG_FILTERSPEC* fileFilter, LPWSTR filePath)
{
	m_hWnd = hWnd;
	m_fileFilter = fileFilter;
	if (!m_fileFilter)
	{
		m_fileFilter = new COMDLG_FILTERSPEC[1];
		m_fileFilter[0].pszName = L"All (*.*)";
		m_fileFilter[0].pszSpec = L"*.*";
	}

	if (filePath)
	{
		// Try loading the file
	}
}

CradleFile::~CradleFile()
{
	if (m_fileFilter) delete [] m_fileFilter;
}

/*

HRESULT CradleFile::OpenPickFileDialog()
{
	CComPtr<IFileOpenDialog> pFileDialog;
	HRESULT hr = pFileDialog.CoCreateInstance(__uuidof(FileOpenDialog));
	HRPRINT(hr, L"Failed to create instance\n", L"Created open file instance\n");

	if (SUCCEEDED(hr))
	{
		hr = pFileDialog->Show(m_hWnd);
		HRPRINT(hr, L"Failed to show open file\n", L"Showing open file\n");
		if (SUCCEEDED(hr))
		{
			IShellItem* pItem;
			hr = pFileDialog->GetResult(&pItem);
			HRPRINT(hr, L"Failed to get item\n", L"Got item\n");
			if (SUCCEEDED(hr))
			{
				LPWSTR str;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &str);
				HRPRINT(hr, L"Failed to get filepath\n", L"Got filepath\n");
				if (SUCCEEDED(hr))
				{
					m_filePath = str;
				}
				else
				{
					// No file name
					m_filePath = nullptr;
				}
			}
			else
			{
				// Cancelled?
			}
		}
		else
		{
			// Failed to open dialog
		}
	}
	else
	{
		// Couldn't create instance for File dialog
	}

	return hr;
	
}

*/