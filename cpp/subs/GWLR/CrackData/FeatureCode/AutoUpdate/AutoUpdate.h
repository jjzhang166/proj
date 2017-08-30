// AutoUpdate.h : main header file for the AutoUpdate DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CAutoUpdateApp
// See AutoUpdate.cpp for the implementation of this class
//

class CAutoUpdateApp : public CWinApp
{
public:
	CAutoUpdateApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
