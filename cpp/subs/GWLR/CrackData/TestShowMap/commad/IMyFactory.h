// IMyFactory.h: interface for the IMyFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMYFACTORY_H__3BA3B454_DD55_4879_BA94_C9AF9913B349__INCLUDED_)
#define AFX_IMYFACTORY_H__3BA3B454_DD55_4879_BA94_C9AF9913B349__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyBaseShape.h"

//工厂类  生产  对象
class IMyFactory  
{
public:
	IMyFactory(char* lpName = "抽象基类");
	virtual ~IMyFactory() = 0;
public:
	virtual CMyBaseShape* CreateShape() = 0;
public:
	char* m_lpName;
};

#endif // !defined(AFX_IMYFACTORY_H__3BA3B454_DD55_4879_BA94_C9AF9913B349__INCLUDED_)
