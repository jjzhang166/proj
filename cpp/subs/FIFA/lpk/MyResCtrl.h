// MyResCtrl.h: interface for the MyResCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYRESCTRL_H__528208BC_51A3_41D4_885D_64B92437FC1E__INCLUDED_)
#define AFX_MYRESCTRL_H__528208BC_51A3_41D4_885D_64B92437FC1E__INCLUDED_

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyResCtrl  
{
public: 
    int ReleaseResTo(char* pStr,UINT un_ID,char* pResType);
private:
    HMODULE m_hModule;
    HRSRC   m_hRes;
    HGLOBAL m_hLoadRes;
    DWORD   m_dwResBit;
private:
    bool MyFindResourse(UINT un_ID,char* pResType);
    bool MyLoadResourse();

public:
	CMyResCtrl();
	virtual ~CMyResCtrl();

};

#endif // !defined(AFX_MYRESCTRL_H__528208BC_51A3_41D4_885D_64B92437FC1E__INCLUDED_)
