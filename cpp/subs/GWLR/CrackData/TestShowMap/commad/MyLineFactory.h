// MyLineFactory.h: interface for the CMyLineFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYLINEFACTORY_H__A148E5E5_D1D4_45A4_87EA_13C9715065D6__INCLUDED_)
#define AFX_MYLINEFACTORY_H__A148E5E5_D1D4_45A4_87EA_13C9715065D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IMyFactory.h"

class CMyLineFactory : public IMyFactory  
{
public:
	CMyLineFactory(char* lpName = "ÏßÌõ");
	virtual ~CMyLineFactory();
public:
	virtual CMyBaseShape* CreateShape();
};

#endif // !defined(AFX_MYLINEFACTORY_H__A148E5E5_D1D4_45A4_87EA_13C9715065D6__INCLUDED_)
