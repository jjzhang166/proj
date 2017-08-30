// MyLineFactory.cpp: implementation of the CMyLineFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyLineFactory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "MyLine.h"
#include "IMyFactory.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyLineFactory::CMyLineFactory(char* lpName)
:IMyFactory(lpName)
{

}

CMyLineFactory::~CMyLineFactory()
{

}

CMyBaseShape* CMyLineFactory::CreateShape()
{
	return new CMyLine;
}
