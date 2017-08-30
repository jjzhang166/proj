// MyLock.h: interface for the CMyLock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYLOCK_H__19918D44_7ADF_4004_B7A0_FCB74B405AB6__INCLUDED_)
#define AFX_MYLOCK_H__19918D44_7ADF_4004_B7A0_FCB74B405AB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyLock  
{
public:
	void Lock(char* pszPrint);
	void UnLock(char* pszPrint);

private:
	HANDLE m_hEvent;
public:
	CMyLock();
	virtual ~CMyLock();

};

#endif // !defined(AFX_MYLOCK_H__19918D44_7ADF_4004_B7A0_FCB74B405AB6__INCLUDED_)
