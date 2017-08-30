#pragma once

class JFZRItem
{
public:
	JFZRItem(void);
	~JFZRItem(void);
public:
	DWORD TraverseItem(int nType);
	DWORD TraverseItem2(int nType);
public:
	CString	csTxt;
};

extern CString	g_csItemPrintStr;