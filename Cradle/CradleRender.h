#ifndef _CRADLERENDER_
#define _CRADLERENDER_

#include "CradleD3D.h"

class CradleRender
{
public:
	CradleRender();
	virtual ~CradleRender();


private:
	CradleD3D* m_cd3d;
};



#endif