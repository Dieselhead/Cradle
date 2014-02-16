#ifndef _CRADLERESOURCE_
#define _CRADLERESOURCE_


#include <string>
#include "Macros.h"

class CradleFileManager;

class CradleResource
{
public:
	CradleResource(std::wstring _name, CradleFileManager* fm);
	virtual ~CradleResource();

	__declspec(dllexport) std::wstring GetName();

public:
	std::wstring m_name;

	CradleFileManager* m_fileManager;
};


#endif