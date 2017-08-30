#pragma once
#include "afx.h"


struct stLogFlush{
	long		line_;
};
stLogFlush MakeLogFlush( long line );
#define MAKE_LOG_FLUSH() MakeLogFlush(__LINE__)

class Log 
{
private:
	char		m_szLog[0x4096];
	CFile		m_theFile;

public:
	Log(void);
	~Log(void);
public:
	bool OpenFile(const char* pszPatch);
	void WriteData();

	void Init()
	{
		memset(m_szLog,0,sizeof(m_szLog));
	}

	Log& operator << (const char* pszLogData)
	{
		auto len = strlen(pszLogData)+1;
		char* pszLog = new char[len];
		if( pszLog != NULL )
		{
			memset(pszLog,0,strlen(pszLogData));
			sprintf_s(pszLog, len, "%s" ,pszLogData);
			strcat_s(m_szLog,pszLog);

			delete pszLog;
			pszLog = NULL;
		}
		else
		{
			ASSERT(false);
		}
		return *this;
	}

	Log& operator << (DWORD dwValue)
	{
		char szLog[64] = {0};
		sprintf_s(szLog,"0x%p",dwValue);
		strcat_s(m_szLog,szLog);
		return *this;
	}
	Log& operator << ( const void* dwValue )
	{
		char szLog[64] = {0};
		sprintf_s(szLog,"0x%p",dwValue);
		strcat_s(m_szLog,szLog);
		return *this;
	}
	Log& operator << ( const stLogFlush& lf ){
		WriteData();
		char buffer[40];
		sprintf_s(buffer, "[LOG]»»ÐÐ%d", lf.line_);
		OutputDebugStringA(buffer);
		return *this;
	}
	template<typename T>
	Log& operator << ( const T& value ){
		return *this << (DWORD)value;
	}

// 	Log& operator << (FLOAT fValue)
// 	{
// 		TCHAR szLog[64] = {0};
// 		_stprintf(szLog,_T("%f"),fValue);
// 		_tcscat(m_szLog,szLog);
//		return *this;
// 	}

};

#define LOG_FILE	"CD_Updates.h"

#define LOG_INIT(s)	Log theLog;\
					theLog.OpenFile(s);\

#define LOG			theLog

#define LOG_END		theLog.WriteData();