// SyncTime.h : main header file for the SYNCTIME application
//

#if !defined(AFX_SYNCTIME_H__C448AC3D_A9C2_4F60_A805_C507155FAFFE__INCLUDED_)
#define AFX_SYNCTIME_H__C448AC3D_A9C2_4F60_A805_C507155FAFFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSyncTimeApp:
// See SyncTime.cpp for the implementation of this class
//

class CSyncTimeApp : public CWinApp
{
public:
	CSyncTimeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSyncTimeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSyncTimeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYNCTIME_H__C448AC3D_A9C2_4F60_A805_C507155FAFFE__INCLUDED_)
