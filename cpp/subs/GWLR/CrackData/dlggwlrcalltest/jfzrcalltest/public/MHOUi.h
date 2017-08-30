#pragma once
#include <WinDef.h>

class MHOUi
{
public:
	MHOUi(void);
	~MHOUi(void);

	static DWORD TraUi(const char *szClassName); //返回类名的地址，如果不可视，或者没找到，返回0
	static BOOL ClickButton(const char*szClassName);
	static BOOL PutString(const char*pszClassName,const wchar_t *pszChar);
private:
	static BOOL PutString(DWORD dwObjAddr,const wchar_t *pszChar);
	static BOOL CleanString(DWORD dwObjAddr);
	static BOOL ClickButton(DWORD dwObjAddr);
	static void GetUIClassName(DWORD dwAddr,char *RetName);
	static BOOL GetUIVisible(DWORD dwAddr);
};

