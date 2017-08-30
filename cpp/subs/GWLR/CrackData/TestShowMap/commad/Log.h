#pragma once
#include "afx.h"

class Log 
{
private:
	char		m_szLog[1024];
	CFile		m_theFile;
public:
	Log(void);
	~Log(void);
public:
	bool OpenFile(const TCHAR* pszPatch);
	void WriteData();
	void Close();

	void Init()
	{
		memset(m_szLog,0,sizeof(m_szLog));
	}

	void operator << (const char* pszLogData)
	{
		auto len = strlen(pszLogData);
		char* pszLog = new char[len + 1];
		if( pszLog != NULL )
		{
			memset(pszLog, 0, len+1);
			sprintf_s(pszLog, len+1, "%s", pszLogData);
			strcat_s(m_szLog,pszLog);

			delete pszLog;
			pszLog = NULL;
		}
		else
		{
			ASSERT(false);
		}
	}

	void operator << (DWORD dwValue)
	{
		char szLog[64] = {0};
		sprintf_s(szLog,"0x%p",dwValue);
		strcat_s(m_szLog,szLog);
	}

// 	void operator << (FLOAT fValue)
// 	{
// 		TCHAR szLog[64] = {0};
// 		_stprintf(szLog,_T("%f"),fValue);
// 		_tcscat(m_szLog,szLog);
// 	}

};

#define LOG_FILE	_T("updates.h")

#define LOG_VALUE	Log theLog;

#define LOG_INIT(s)	theLog.OpenFile(s);\

#define LOG			theLog

#define LOG_END		theLog.WriteData();

#define LOG_CLOSE	theLog.Close();