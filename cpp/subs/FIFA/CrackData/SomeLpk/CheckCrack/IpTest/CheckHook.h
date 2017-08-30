#pragma once

class CheckHook
{
public:
	CheckHook(void);
	~CheckHook(void);
	bool CheckInlineHook(const char* pszModuleName,const char* pszFunName);
};
