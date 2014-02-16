#include "CradleResource.h"
#include "CradleFileManager.h"

CradleResource::CradleResource(std::wstring _name, CradleFileManager* fm)
{
	m_name = _name;
	m_fileManager = fm;
}

CradleResource::~CradleResource()
{

}

std::wstring CradleResource::GetName()
{
	return m_name;
}
